/* Automatically generated by refactor.pl.
 *
 *
 * CMDNAME: listflags
 * CMDLEVEL: QCMD_OPER
 * CMDARGS: 1
 * CMDDESC: List users with the specified user flags.
 * CMDFUNC: csu_dolistflags
 * CMDPROTO: int csu_dolistflags(void *source, int cargc, char **cargv);
 * CMDHELP: Usage: listflags <flags>
 * CMDHELP: Shows a list of all users with the specified user flags.
 */

#include "../chanserv.h"
#include "../../lib/irc_string.h"
#include <stdio.h>
#include <string.h>

int csu_dolistflags(void *source, int cargc, char **cargv) {
  nick *sender=source;
  reguser *rup=getreguserfromnick(sender);
  reguser *dbrup;
  flag_t matchflags = 0;
  char *ch;
  int i, j;
  unsigned int count=0;
  
  if (!rup)
    return CMD_ERROR;
  
  if (cargc < 1) {
    chanservstdmessage(sender, QM_NOTENOUGHPARAMS, "listflags");
    return CMD_ERROR;
  }
  
  ch=cargv[0][0]=='+'?cargv[0]+1:cargv[0];
  
  for (i=0; ch[i]; i++) {
    for (j = 0; ruflags[j].flagchar; j++) {
      if (ruflags[j].flagchar == ch[i]) {
        matchflags|=ruflags[j].flagbit;
        break;
      }
    }
  }

  if (!matchflags || (matchflags == QUFLAG_NOTICE)) {
    chanservsendmessage(sender, "Error: no flags selected");
    return CMD_ERROR;
  }

  chanservstdmessage(sender, QM_LISTFLAGSHEADER);
  for (i=0;i<REGUSERHASHSIZE;i++) {
    for (dbrup=regusernicktable[i]; dbrup; dbrup=dbrup->nextbyname) {
      if ((dbrup->flags & matchflags) == matchflags) {
        char tbuf[TIMELEN], *tdata;

        if (dbrup->lastemailchange) {
          q9strftime(tbuf, sizeof(tbuf), dbrup->lastemailchange);
          tdata = tbuf;
        } else {
          tdata = "(none)";
        }

        char *email;
        if (dbrup->email && dbrup->email->content[0]) {
          email = dbrup->email->content;
        } else {
          email = "(none)";
        }

        chanservsendmessage(sender, " %-15s %-10u %-17s %-30s %-15s", dbrup->username, dbrup->ID, printflags(dbrup->flags, ruflags), email, tdata);
        count++;
        if (count >= 2000) {
          chanservstdmessage(sender, QM_TOOMANYRESULTS, 2000, "users");
          return CMD_ERROR;
        }
      }
    }
  }
  chanservstdmessage(sender, QM_RESULTCOUNT, count, "user", (count==1)?"":"s");
  
  return CMD_OK;
}
