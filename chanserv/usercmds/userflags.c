/* Automatically generated by refactor.pl.
 *
 *
 * CMDNAME: userflags
 * CMDLEVEL: QCMD_AUTHED
 * CMDARGS: 2
 * CMDDESC: Shows or changes user flags.
 * CMDFUNC: csu_douserflags
 * CMDPROTO: int csu_douserflags(void *source, int cargc, char **cargv);
 * CMDHELP: Usage: USERFLAGS <flags>
 * CMDHELP: Changes your current user flags, where:
 * CMDHELP: flags - changes to apply, in the usual flag letters preceded by +/- format.
 * CMDHELP: Valid user flags are:
 * CMDHELP:  +n NOTICE  - causes the bot to sent you NOTICEs.  If this flag is not set the
 * CMDHELP:               bot will communicate using PRIVMSG.
 */

#include "../chanserv.h"
#include "../../lib/irc_string.h"
#include <stdio.h>
#include <string.h>

int csu_douserflags(void *source, int cargc, char **cargv) {
  nick *sender=source;
  reguser *rup=getreguserfromnick(sender), *target;
  int arg=0;
  flag_t flagmask, changemask, oldflags;
  char flagbuf[30];

  if (!rup)
    return CMD_ERROR;
  
  if (cargc>0 && (*cargv[0]!='+' && *cargv[0]!='-')) {
    arg++;
    /* If the first char isn't a "change" character, it must specify a target */

    if (!(target=findreguser(sender,cargv[0])))
      return CMD_ERROR;

    if (target!=rup && !cs_privcheck(QPRIV_VIEWUSERFLAGS, sender)) {
      chanservstdmessage(sender, QM_NOACCESSONUSER, "userflags", cargv[0]);
      return CMD_ERROR;
    }
  } else {
    target=rup;
  }
    
  if (cargc>arg) {
    /* OK, now we have a changestring.. */
    if (target!=rup && !cs_privcheck(QPRIV_CHANGEUSERFLAGS, sender)) {
      /* Safe to use cargv[0] because if target != rup then first arg must have been the target name */
      chanservstdmessage(sender, QM_NOACCESSONUSER, "userflags", cargv[0]);
      return CMD_ERROR;
    }

    strcpy(flagbuf,printflags(target->flags, ruflags));
    oldflags=target->flags;

    changemask=QUFLAG_NOTICE | QUFLAG_INFO;

    if (target==rup) {
      /* If you're changing yourself, you can give up the "status" flags and add/remove notice */
      changemask|=(target->flags & (QUFLAG_OPER | QUFLAG_DEV | QUFLAG_PROTECT | QUFLAG_HELPER | QUFLAG_ADMIN));
    }

    /* Warning, policy ahead */

    if (UHasOperPriv(rup))
      changemask |= QUFLAG_GLINE | QUFLAG_DELAYEDGLINE | QUFLAG_PROTECT | QUFLAG_TRUST;

    if (UHasAdminPriv(rup))
      changemask |= (QUFLAG_OPER | QUFLAG_HELPER | QUFLAG_CLEANUPEXEMPT);
    
    if (UIsDev(rup))
      changemask=QUFLAG_ALL;
    
    setflags(&target->flags, changemask, cargv[arg], ruflags, REJECT_NONE);
    
    /* More policy */
    if (!UHasHelperPriv(target)) {
      target->flags &= ~QUFLAG_PROTECT;
    }

    cs_log(sender,"USERFLAGS #%s %s (%s -> %s)",target->username,cargv[arg],flagbuf,printflags(target->flags, ruflags));

    /* only warn about interesting changes */
    if((target->flags ^ oldflags) & ~(QUFLAG_NOTICE | QUFLAG_INFO))
      chanservwallmessage("%s (%s) just used USERFLAGS on %s %s (%s -> %s)",sender->nick,rup->username,target->username,cargv[arg],flagbuf,printflags(target->flags,ruflags));

    csdb_updateuser(target);
    chanservstdmessage(sender, QM_DONE);
  }

  if (cs_privcheck(QPRIV_VIEWUSERFLAGS, sender))
    flagmask=QUFLAG_ALL;
  else
    flagmask=QUFLAG_INFO | QUFLAG_NOTICE | QUFLAG_OPER | QUFLAG_HELPER | QUFLAG_DEV | QUFLAG_ADMIN;
  
  chanservstdmessage(sender, QM_CURUSERFLAGS, target->username, printflagsornone(target->flags & flagmask, ruflags));

  return CMD_OK;
}
