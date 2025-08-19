/* vi normal mode controls */
struct vi_normal_con {
  int sti; /* switch to insert        */
  int stt; /* switch to real terminal */
  int stv; /* switch to vi terminal   */
  int qts; /* quit this session       */
};

/* vi insert controls */
struct vi_insert_con {
  int lcm; /* leave current mode   */
  int cel; /* clear to end of line */
  int dlw; /* delete word */
};

/* vi real terminal controls */
struct vi_rterm_con {
  int lcm; /* leave current mode */
};

/* vi editor terminal controls */
struct vi_term_con {
  int lcm; /* leave current mode */
};
