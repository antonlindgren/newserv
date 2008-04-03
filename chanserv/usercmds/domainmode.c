/* Automatically generated by refactor.pl.
 *
 *
 * CMDNAME: domainmode
 * CMDLEVEL: QCMD_OPER
 * CMDARGS: 4
 * CMDDESC: Set/Unset Mail Domain Modes
 * CMDFUNC: csu_dodomainmode
 * CMDPROTO: int csu_dodomainmode(void *source, int cargc, char **cargv);
 * CMDHELP: Usage: domainmode <domain> [<flags>]
 * CMDHELP: Views or modifies the domainmode flags for specified domain.
 * CMDHELP: Flags consist of:
 * CMDHELP:   +b          don't allow email addresses from this domain.
 * CMDHELP:   +u <limit>  don't allow more than <limit> accounts to share
 * CMDHELP:               email addresses on this domain.
 * CMDHELP:   +l <limit>  don't allow more than <limit> accounts with
 * CMDHELP:               email addresses on this domain.
 * CMDHELP: Note that domains are hierarchical, so setting +b on org will
 * CMDHELP: prevent operations from quakenet.org, fish.quakenet.org, etc.
 */

#include "../chanserv.h"
#include "../../lib/irc_string.h"
#include <stdio.h>
#include <string.h>

char *getdomainmode(maildomain *mdp) {
  static char buf1[50];
  char buf2[30];

  if (mdp->flags) {
    strcpy(buf1,printflags(mdp->flags, mdflags));
  } else {
    buf1[0]='\0';
  }

  if (mdp->flags & MDFLAG_LIMIT) {
    sprintf(buf2, " %d",mdp->limit);
    strcat(buf1, buf2);
  }

  if (mdp->flags & MDFLAG_ACTLIMIT) {
    sprintf(buf2, " %d",mdp->actlimit);
    strcat(buf1, buf2);
  }

  if (*buf1=='\0') {
    strcpy(buf1,"(none)");
  }

  return buf1;
}

int checkdomain(char *dom) {
  const char *validchars="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
  int hypok=0;
  char lastch;
  char *ch;
  
  for (ch=dom;*ch;ch++) {
    if (*ch=='.') {
      if (hypok==0) {
        /* This is two dots in a row - bad */
        return 1;
      }
      hypok=0;
    } else if (*ch=='-') {
      if (hypok==0) {
        /* either .- or - at the start - bad */
        return 1;
      }
    } else {
      if (!strchr(validchars, *ch)) {
        /* Unrecognized char */
        return 1;
      }
     
      /* We encountered some random valid characters, so hyphens (and dots) are now OK until the next dot */ 
      hypok=1;
    }
    lastch=*ch;
  }
  
  if (lastch=='.') {
    /* Can't end on a dot */
    return 1;
  }
  
  return 0;
}

int csu_dodomainmode(void *source, int cargc, char **cargv) {
  maildomain *mdp; 
  nick *sender=source;
  flag_t forceflags, currentflags;
  char buf1[60];
  int carg=2,limdone=0,actlimdone=0;
  unsigned int newlim=0;
  unsigned int newactlim=0;

  if (cargc<1) {
    chanservstdmessage(sender,QM_NOTENOUGHPARAMS,"domainmode");
    return CMD_ERROR;
  }

  if (checkdomain(cargv[0])) {
    chanservstdmessage(sender,QM_INVALIDDOMAIN,cargv[0]);
    return CMD_ERROR;
  }

  if(!(mdp=findorcreatemaildomain(cargv[0]))) {
    return CMD_ERROR;
  }

  if (cargc>1) {
    /* Save the current modes.. */
    strcpy(buf1,getdomainmode(mdp));

    /* Pick out the + flags: start from 0 */
    forceflags=0;
    setflags(&forceflags, MDFLAG_ALL, cargv[1], mdflags, REJECT_NONE);
    currentflags=mdp->flags;
    setflags(&currentflags, MDFLAG_ALL, cargv[1], mdflags, REJECT_NONE);

    if ((forceflags & MDFLAG_LIMIT) &&
        (!(forceflags & MDFLAG_ACTLIMIT) || strrchr(cargv[1],'l') < strrchr(cargv[1],'u'))) {
      if (cargc<=carg) {
        chanservstdmessage(sender,QM_NOTENOUGHPARAMS,"domainmode");
        return CMD_ERROR;
      }
      newlim=strtol(cargv[carg++],NULL,10);
      limdone=1;
    }

    if ((forceflags & MDFLAG_LIMIT) && !limdone) {
      if (cargc<=carg) {
        chanservstdmessage(sender,QM_NOTENOUGHPARAMS,"domainmode");
        return CMD_ERROR;
      }
      newlim=strtol(cargv[carg++],NULL,10);
      limdone=1;
    }

    if ((forceflags & MDFLAG_ACTLIMIT) && !actlimdone) {
      if (cargc<=carg) {
        chanservstdmessage(sender,QM_NOTENOUGHPARAMS,"chanmode");
        return CMD_ERROR;
      }
      newactlim=strtol(cargv[carg++],NULL,10);
      actlimdone=1;
    }

    /* It parsed OK, so update the structure.. */
    mdp->flags=currentflags;
    if(actlimdone)
      mdp->actlimit=newactlim; 
    if(!(currentflags & MDFLAG_ACTLIMIT))
      mdp->actlimit=0;
    if(limdone)
      mdp->limit=newlim;
    if(!(currentflags & MDFLAG_LIMIT))
      mdp->limit=0;
    if(mdp->ID) {
      if(mdp->flags) {
        csdb_updatemaildomain(mdp);
      } else {
        csdb_deletemaildomain(mdp);
      }
    } else {
      mdp->ID=++lastdomainID;
      csdb_createmaildomain(mdp);
    }

    chanservstdmessage(sender, QM_DONE);
    cs_log(sender,"DOMAINMODE %s %s (%s -> %s)",mdp->name->content,cargv[1],buf1,getdomainmode(mdp));
  }
  chanservstdmessage(sender,QM_CURDOMAINMODES,mdp->name->content,getdomainmode(mdp));
 
  return CMD_OK;
}
