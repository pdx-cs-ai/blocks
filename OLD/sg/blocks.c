#include "blocks.h"

/* global state */
int n_blocks;
struct state *start, *goal;
/* statistics */
int stat_nodes = 1;
int stat_cached = 1;
int stat_open = 1;
int stat_max_open = 1;
int stat_max_g = 0;
int stat_min_h;
int stat_decision_nodes = -1;
/* control */
int verbose = 0;
int node_limit = 0;
int timer_expired = 0;
int fast_heuristic = 1;

int main(int argc, char **argv) {
  int ida = 0;
  int rida = 0;
  int ra = 0;

  argv++; argc--;
  while (argc > 0)
    if (argv[0][0] != '-')
      abort();
    else
      switch (argv[0][1]) {
      case 'v':
	if (argc < 2)
	  abort();
	verbose = atoi(argv[1]);
	argv += 2; argc -= 2;
	break;
      case 'l':
	if (argc < 2)
	  abort();
	node_limit = atoi(argv[1]);
	argv += 2; argc -= 2;
	break;
      case 't':
	if (argc < 2)
	  abort();
	set_timer(atoi(argv[1]));
	argv += 2; argc -= 2;
	break;
      case 'a':
	if (argc < 2)
	  abort();
	if (!strcmp(argv[1], "ida"))
	  ida = 1;
	else if (!strcmp(argv[1], "rida"))
	  rida = 1;
	else if (!strcmp(argv[1], "ra"))
	  ra = 1;
	else
	  abort();
	argv += 2; argc -= 2;
	break;
      case 'h':
	fast_heuristic = 0;
	argv++; argc--;
	break;
      }
  read_problem();
  if (fast_heuristic)
    score_state(start);
  else
    deluxe_score_state(start);
  start->g_score = 0;
  start->t_score = start->h_score;
  hash_state(start);
  start->parent = 0;
  start->moved_block = -1;
  start->moved_to = -1;
  goal->h_score = 0;   /* better be! */
  hash_state(goal);
  stat_min_h = start->h_score;
  if (ida)
    return ida_star();
  if (rida)
    return rida_star();
  if (ra)
    return ra_star();
  return a_star();
}
