char *pr_str(MalDatum x) {
	switch (x.tag) {
	case msymbol:
		return x.string;
	case mnum:
		
