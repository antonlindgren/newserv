/* Automatically generated by refactor.pl.
 *
 *
 * CMDNAME: whois
 * CMDLEVEL: QCMD_AUTHED
 * CMDARGS: 1
 * CMDDESC: Displays information about a user.
 * CMDFUNC: csu_dowhois
 * CMDPROTO: int csu_dowhois(void *source, int cargc, char **cargv);
 * CMDHELP: Usage: WHOIS <user>
 * CMDHELP: Displays information about a user, where:
 * CMDHELP: user  - user to request information for, either the nickname of an active user on
 * CMDHELP:         the network or #accountname.
 */

#include "../chanserv.h"
#include "../../lib/irc_string.h"
#include "../../lib/strlfunc.h"
#include <stdio.h>
#include <string.h>

int csu_dowhois(void *source, int cargc, char **cargv) {
  nick *sender=source;
  reguser *rup=getreguserfromnick(sender), *target;
  char buf[200];
  char nbpos=0;
  struct tm *tmp;
  regchanuser *rcup, *rcup2;
  flag_t flagmask, flags;
  int doneheader=0;
  authname *anp;
  nick *tnp;

  if (!(rup=getreguserfromnick(sender)))
    return CMD_ERROR;
  
  if (cargc<1) {
    chanservstdmessage(sender, QM_NOTENOUGHPARAMS, "whois");
    return CMD_ERROR;
  }
  
  if (!(target=findreguser(sender, cargv[0]))) {
    nick *np;
    
    if (cs_privcheck(QPRIV_VIEWFULLWHOIS, sender) && (np=getnickbynick(cargv[0]))) {
      activeuser *aup=getactiveuserfromnick(np);
      chanservsendmessage(sender, "%s has attempted to auth %d time%s.", np->nick, aup->authattempts, 
        aup->authattempts==1?"":"s");
    }
    return CMD_ERROR;
  }

  if (cargv[0][0]=='#') {
    chanservstdmessage(sender, QM_WHOISHEADER_AUTH, target->username);
  } else {
    chanservstdmessage(sender, QM_WHOISHEADER_NICK, cargv[0], target->username);
  }

  if (rup==target || cs_privcheck(QPRIV_VIEWFULLWHOIS, sender)) {
    chanservstdmessage(sender, QM_WHOIS_USERID, target->ID);
  }

  if (cs_privcheck(QPRIV_VIEWUSERFLAGS, sender)) {
    flagmask=QUFLAG_ALL & ~(QUFLAG_SUSPENDED | QUFLAG_GLINE | QUFLAG_DELAYEDGLINE);
    if (UIsDev(rup))
      flagmask=QUFLAG_ALL;

    if (UHasSuspension(target)) {
      char expiresbuf[100], timebuf[100];
      char *reason, *suspendtype, *whom;

      if(UIsDelayedGline(target)) {
        suspendtype = "delayed gline";
      } else if(UIsGline(target)) {
        suspendtype = "gline";
      } else if(UIsSuspended(target)) {
        suspendtype = "suspended";
      } else {
        suspendtype = "???";
      }

      if(target->suspendexp) {
        if(time(NULL) >= target->suspendexp) {
          strlcpy(expiresbuf, "(next auth)", sizeof(expiresbuf));
        } else {
          strftime(expiresbuf, 15, "%d/%m/%y %H:%M", gmtime(&(target->suspendexp)));
        }
      } else {
        strlcpy(expiresbuf, "(never)", sizeof(expiresbuf));
      }
      strftime(timebuf, 15, "%d/%m/%y %H:%M", gmtime(&(target->suspendtime)));

      if(UHasOperPriv(rup)) {
        reguser *trup = findreguserbyID(target->suspendby);
        if(trup) {
          whom = trup->username;
        } else {
          whom = "(unknown)";
        }
      } else {
        whom = "(hidden)";
      }

      if(target->suspendreason && target->suspendreason->content) {
        reason = target->suspendreason->content;
      } else {
        reason = "(none)";
      }

      chanservstdmessage(sender, QM_USERSUSPENDEDTYPE, suspendtype);
      chanservstdmessage(sender, QM_USERSUSPENDEDBY, whom);
      chanservstdmessage(sender, QM_USERSUSPENDEDREASON, reason);
      chanservstdmessage(sender, QM_USERSUSPENDEDAT, timebuf);
      chanservstdmessage(sender, QM_USERSUSPENDEDEXPIRY, expiresbuf);
    }
  } else {  
    /* Incidentally none of the QM_USERIS* mesages take parameters.  Just thought I'd mention it.. */
    if (UIsAdmin(target))
      chanservstdmessage(sender, QM_USERISADMIN, target->username);
    else if (UIsOper(target))
      chanservstdmessage(sender, QM_USERISOPER, target->username);
    else if (UIsHelper(target))
      chanservstdmessage(sender, QM_USERISHELPER, target->username);
    else if (UIsStaff(target))
      chanservstdmessage(sender, QM_USERISSTAFF, target->username);
    
    if (UIsDev(target))
      chanservstdmessage(sender, QM_USERISDEV, target->username);

    flagmask=0;
  }

  if (rup==target)
    flagmask|=(QUFLAG_OPER | QUFLAG_DEV | QUFLAG_HELPER | 
	       QUFLAG_ADMIN | QUFLAG_INFO | QUFLAG_NOTICE | QUFLAG_STAFF);

  if (flagmask & target->flags)
    chanservstdmessage(sender, QM_WHOIS_FLAGS, printflagsornone(flagmask & target->flags, ruflags));

  if (!(anp=findauthname(target->ID)) || !anp->nicks) {
    chanservstdmessage(sender, QM_WHOIS_USERS, "(none)");
  } else {
    for (tnp=anp->nicks; ;tnp=tnp->nextbyauthname) {
      if (nbpos>0 && (!tnp || nbpos+strlen(tnp->nick) > 60)) {
	chanservstdmessage(sender, QM_WHOIS_USERS, buf);
	nbpos=0;
      }

      if (!tnp)
	break;

      nbpos+=sprintf(buf+nbpos,"%s ",tnp->nick);
    }
  }

  if (target->created) {
    tmp=gmtime(&(target->created));
    strftime(buf,15,"%d/%m/%y %H:%M",tmp);
    
    chanservstdmessage(sender, QM_WHOIS_CREATED, buf);
  }

  if (target->lastauth == 0) {
    snprintf(buf,sizeof(buf),"(never)");
  } else {
    tmp=gmtime(&(target->lastauth));
    strftime(buf,15,"%d/%m/%y %H:%M",tmp);
  }
  chanservstdmessage(sender, QM_WHOIS_LASTAUTH, buf);
  
  if (target->lastuserhost && (rup==target || cs_privcheck(QPRIV_VIEWFULLWHOIS, sender))) {
/* Commenting out language until we implement some - splidge 
    chanservstdmessage(sender, QM_WHOIS_USERLANG, cslanguages[target->languageid] ?
		       cslanguages[target->languageid]->name->content : "(unknown)");  */
    chanservstdmessage(sender, QM_WHOIS_LASTUSERHOST, target->lastuserhost->content);
  }

  if (target->email && (rup==target || cs_privcheck(QPRIV_VIEWEMAIL, sender))) {
    chanservstdmessage(sender, QM_WHOIS_EMAIL, target->email->content);

    tmp=gmtime(&(target->lastemailchange));
    strftime(buf,15,"%d/%m/%y %H:%M",tmp);
    
    chanservstdmessage(sender, QM_WHOIS_EMAILSET, buf);    
  }

  if (target->info && *target->info->content) {
    chanservstdmessage(sender, QM_WHOIS_INFO, target->info->content);
  }

  if (target->comment && (cs_privcheck(QPRIV_VIEWCOMMENTS, sender))) {
    chanservstdmessage(sender, QM_WHOIS_COMMENT, target->comment->content);
  }
  
  for (rcup=target->knownon;rcup;rcup=rcup->nextbyuser) {
    if (!UHasHelperPriv(rup) || (UHasOperPriv(target) && UHasHelperPriv(rup) && !UHasOperPriv(rup))) {
      if (!(rcup2=findreguseronchannel(rcup->chan,rup)))
	continue;
      
      if (!CUKnown(rcup2))
	continue;
      
      flagmask = (QCUFLAG_OWNER | QCUFLAG_MASTER | QCUFLAG_OP | QCUFLAG_VOICE | QCUFLAG_AUTOVOICE | 
		  QCUFLAG_AUTOOP | QCUFLAG_TOPIC | QCUFLAG_KNOWN | QCUFLAG_PROTECT );
      
      if (CUHasMasterPriv(rcup2))
	flagmask |= QCUFLAGS_PUNISH;
      
      if (rcup2==rcup)
        flagmask |= QCUFLAGS_PERSONAL;
    } else {
      flagmask=QCUFLAG_ALL;
    }

    if (!(flags=rcup->flags & flagmask)) 
      continue;

    if (!doneheader) {
      doneheader=1;
      chanservstdmessage(sender, QM_WHOIS_CHANHEADER, target->username);
    }

    chanservsendmessage(sender, " %-30s %s",rcup->chan->index->name->content,printflags(flags, rcuflags));
  }

  chanservstdmessage(sender, QM_ENDOFLIST);

  return CMD_OK;
}
