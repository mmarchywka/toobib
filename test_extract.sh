
 ./toobib.out -cmd "aux copper/copper.aux" -cmd "dump-ragged " -cmd "load-bibs foo copper/pmc_copper.bib copper/non_pmc_copper.bib copper/releases.bib" -cmd "dump-mao " -cmd list -cmd "extract-bibs used foo  " -cmd "dump-map used"  -cmd "get-replays replayrag used" -cmd list -cmd quiet-pawn

# ./toobib.out -cmd "aux copper/copper.aux" -cmd "dump-ragged " -cmd "load-bibs foo copper/pmc_copper.bib copper/non_pmc_copper.bib" -cmd "map-dump foo" -cmd list -cmd "extract-bibs used foo  "  -cmd "get-replays replayrag foo" -cmd list


# ./toobib.out -cmd "aux copper/copper.aux" -cmd "dump-ragged " -cmd "load-bibs foo copper/pmc_copper.bib copper/non_pmc_copper.bib" -cmd "map-dump foo" -cmd list -cmd "extract-bibs used foo  " -list

