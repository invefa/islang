manifest(end, 0, 0)
manifest(pass, 0, 0)

manifest(repeat, -1, 0)
manifest(ext, -1, 0)


manifest(jmp, 1, 0)
manifest(cjmp, 1, -1)
manifest(ncjmp, 1, -1)
manifest(goto, 1, 0)
manifest(cgoto, 1, -1)
manifest(ncgoto, 1, -1)


manifest(call, 2, -1)
manifest(callx, 2, -1)
manifest(invoke, 1, -1)
manifest(prep, 1, 0)
manifest(ret, 1, -1)

manifest(pshc, 1, +1)
manifest(pop, 0, -1)
manifest(ldv, 1, +1)
manifest(stv, 1, -1)

manifest(addi, 0, -1)
manifest(subi, 0, -1)
manifest(muli, 0, -1)
manifest(divi, 0, -1)
manifest(modi, 0, -1)

manifest(divu, 0, -1)
manifest(modu, 0, -1)

manifest(i64to32, 0, 0)
manifest(i64to16, 0, 0)
manifest(i64to8, 0, 0)
manifest(u64to32, 0, 0)
manifest(u64to16, 0, 0)
manifest(u64to8, 0, 0)

manifest(addf, 0, -1)
manifest(subf, 0, -1)
manifest(mulf, 0, -1)
manifest(divf, 0, -1)

manifest(addd, 0, -1)
manifest(subd, 0, -1)
manifest(muld, 0, -1)
manifest(divd, 0, -1)


manifest(band, 0, -1)
manifest(bor, 0, -1)
manifest(bxor, 0, -1)
manifest(bnot, 0, 0)
manifest(bshli, 0, 0)
manifest(bshri, 0, 0)
manifest(bshru, 0, 0)


manifest(land, 0, -1)
manifest(lor, 0, -1)
manifest(lnot, 0, 0)


manifest(eq, 0, 0)
manifest(neq, 0, 0)

manifest(lti, 0, 0)
manifest(gti, 0, 0)
manifest(leti, 0, 0)
manifest(geti, 0, 0)

manifest(ltu, 0, 0)
manifest(gtu, 0, 0)
manifest(letu, 0, 0)
manifest(getu, 0, 0)

manifest(ltf, 0, 0)
manifest(gtf, 0, 0)
manifest(letf, 0, 0)
manifest(getf, 0, 0)

manifest(ltd, 0, 0)
manifest(gtd, 0, 0)
manifest(letd, 0, 0)
manifest(getd, 0, 0)
