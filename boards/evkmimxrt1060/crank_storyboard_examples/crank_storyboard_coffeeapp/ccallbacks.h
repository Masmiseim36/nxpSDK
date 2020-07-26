#include <gre/gre.h>
void cbResetSizes(gr_action_context_t *action_context);
void cbResetStart(gr_action_context_t *action_context);
void cbResetBrew(gr_action_context_t *action_context);
void cbResetBrewing(gr_action_context_t *action_context);
sb_ccallback_t clist[] = {
	{ "cbResetSizes",	&cbResetSizes },
	{ "cbResetStart",	&cbResetStart },
	{ "cbResetBrewing", &cbResetBrewing },
	{ "cbResetBrew",	&cbResetBrew },
	{ NULL, NULL },
};