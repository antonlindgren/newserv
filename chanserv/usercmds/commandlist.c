/* Automatically generated by mkcommandlist.pl, do not edit. */

#include "../chanserv.h"

/* Prototypes */
int csa_doaccounthistory(void *source, int cargc, char **cargv);
int csu_docleanupdb(void *source, int cargc, char **cargv);
int csu_dodeluser(void *source, int cargc, char **cargv);
int csu_dodomainmode(void *source, int cargc, char **cargv);
int csu_doinfo(void *source, int cargc, char **cargv);
int csu_dolanguage(void *source, int cargc, char **cargv);
int csu_dolistflags(void *source, int cargc, char **cargv);
int csa_dorollbackaccount(void *source, int cargc, char **cargv);
int csu_dospewdb(void *source, int cargc, char **cargv);
int csu_dospewdomain(void *source, int cargc, char **cargv);
int csu_dospewemail(void *source, int cargc, char **cargv);
int csu_dospewpass(void *source, int cargc, char **cargv);
int csu_dosuspenduser(void *source, int cargc, char **cargv);
int csu_dosuspenduserlist(void *source, int cargc, char **cargv);
int csu_dounsuspenduser(void *source, int cargc, char **cargv);
int csu_dousercomment(void *source, int cargc, char **cargv);
int csu_douserflags(void *source, int cargc, char **cargv);
int csu_dowhoami(void *source, int cargc, char **cargv);
int csu_dowhois(void *source, int cargc, char **cargv);

void _init() {
  chanservaddcommand("accounthistory", QCMD_OPER, 1, csa_doaccounthistory, "View password/email history for an account.");
  chanservaddcommand("cleanupdb", QCMD_OPER, 0, csu_docleanupdb, "Clean Up Db");
  chanservaddcommand("deluser", QCMD_OPER, 2, csu_dodeluser, "Removes a user from the bot.");
  chanservaddcommand("domainmode", QCMD_OPER, 4, csu_dodomainmode, "Set/Unset Mail Domain Modes");
  chanservaddcommand("info", QCMD_OPER | QCMD_AUTHED, 2, csu_doinfo, "Shows or changes info line.");
  chanservaddcommand("language", QCMD_AUTHED, 1, csu_dolanguage, "Shows or changes your current language.");
  chanservaddcommand("listflags", QCMD_OPER, 1, csu_dolistflags, "List users with the specified user flags.");
  chanservaddcommand("rollbackaccount", QCMD_OPER, 2, csa_dorollbackaccount, "Roll back password/email changes on an account.");
  chanservaddcommand("spewdb", QCMD_OPER, 1, csu_dospewdb, "Search for a user in the database.");
  chanservaddcommand("spewdomain", QCMD_OPER, 1, csu_dospewdomain, "Spew Mail Domains.");
  chanservaddcommand("spewemail", QCMD_OPER, 1, csu_dospewemail, "Search for an e-mail in the database.");
  chanservaddcommand("spewpass", QCMD_OPER, 1, csu_dospewpass, "Search for a password in the database.");
  chanservaddcommand("suspenduser", QCMD_OPER, 1, csu_dosuspenduser, "Suspend/Delay GLINE/Instantly GLINE a user.");
  chanservaddcommand("suspenduserlist", QCMD_HELPER, 1, csu_dosuspenduserlist, "Lists suspended/locked users.");
  chanservaddcommand("unsuspenduser", QCMD_OPER, 1, csu_dounsuspenduser, "Unsuspend a user.");
  chanservaddcommand("usercomment", QCMD_OPER, 2, csu_dousercomment, "Shows or changes staff comment for a user.");
  chanservaddcommand("userflags", QCMD_AUTHED, 2, csu_douserflags, "Shows or changes user flags.");
  chanservaddcommand("whoami", QCMD_AUTHED, 0, csu_dowhoami, "Displays information about you");
  chanservaddcommand("whois", QCMD_AUTHED, 1, csu_dowhois, "Displays information about a user.");
}

void _fini() {
  chanservremovecommand("accounthistory", csa_doaccounthistory);
  chanservremovecommand("cleanupdb", csu_docleanupdb);
  chanservremovecommand("deluser", csu_dodeluser);
  chanservremovecommand("domainmode", csu_dodomainmode);
  chanservremovecommand("info", csu_doinfo);
  chanservremovecommand("language", csu_dolanguage);
  chanservremovecommand("listflags", csu_dolistflags);
  chanservremovecommand("rollbackaccount", csa_dorollbackaccount);
  chanservremovecommand("spewdb", csu_dospewdb);
  chanservremovecommand("spewdomain", csu_dospewdomain);
  chanservremovecommand("spewemail", csu_dospewemail);
  chanservremovecommand("spewpass", csu_dospewpass);
  chanservremovecommand("suspenduser", csu_dosuspenduser);
  chanservremovecommand("suspenduserlist", csu_dosuspenduserlist);
  chanservremovecommand("unsuspenduser", csu_dounsuspenduser);
  chanservremovecommand("usercomment", csu_dousercomment);
  chanservremovecommand("userflags", csu_douserflags);
  chanservremovecommand("whoami", csu_dowhoami);
  chanservremovecommand("whois", csu_dowhois);
}
