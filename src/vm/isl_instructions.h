manifest(end, (), 0)
manifest(pass, (), 0)

manifest(repeat, (), 0)
manifest(ext, (), 0)


manifest(jmp, (i16), 0)
manifest(cjmp, (i16), -1)
manifest(ncjmp, (i16), -1)
manifest(goto, (u32), 0)
manifest(cgoto, (u32), -1)
manifest(ncgoto, (u32), -1)


manifest(call, (), -1)
manifest(callx, (), -1)
manifest(invoke, (), -1)
manifest(prep, (), 0)
manifest(ret, (), -1)

manifest(pshc, (u32), +1)
manifest(pshi, (i64), +1)
manifest(pshu, (u64), +1)
manifest(pshf, (f32), +1)
manifest(pshd, (f64), +1)
manifest(pop, (), -1)
manifest(ldv, (u16), +1)
manifest(stv, (u16), -1)

manifest(addi, (), -1)
manifest(subi, (), -1)
manifest(muli, (), -1)
manifest(divi, (), -1)
manifest(modi, (), -1)

manifest(divu, (), -1)
manifest(modu, (), -1)

manifest(i64t32, (), 0)
manifest(i64t16, (), 0)
manifest(i64t8, (), 0)
manifest(u64t32, (), 0)
manifest(u64t16, (), 0)
manifest(u64t8, (), 0)
manifest(f64t32, (), 0)

manifest(addf, (), -1)
manifest(subf, (), -1)
manifest(mulf, (), -1)
manifest(divf, (), -1)

manifest(addd, (), -1)
manifest(subd, (), -1)
manifest(muld, (), -1)
manifest(divd, (), -1)


manifest(band, (), -1)
manifest(bor, (), -1)
manifest(bxor, (), -1)
manifest(bnot, (), 0)
manifest(bshli, (), 0)
manifest(bshri, (), 0)
manifest(bshru, (), 0)


manifest(land, (), -1)
manifest(lor, (), -1)
manifest(lnot, (), 0)


manifest(eq, (), 0)
manifest(neq, (), 0)

manifest(lti, (), 0)
manifest(gti, (), 0)
manifest(leti, (), 0)
manifest(geti, (), 0)

manifest(ltu, (), 0)
manifest(gtu, (), 0)
manifest(letu, (), 0)
manifest(getu, (), 0)

manifest(ltf, (), 0)
manifest(gtf, (), 0)
manifest(letf, (), 0)
manifest(getf, (), 0)

manifest(ltd, (), 0)
manifest(gtd, (), 0)
manifest(letd, (), 0)
manifest(getd, (), 0)
