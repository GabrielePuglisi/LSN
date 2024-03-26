double mutation=rnd.Rannyu(0,4);
        double prob=rnd.Rannyu(0,p);
        if(mutation<1)
            PairPermutation(a);

        if(mutation>=1 and mutation<2); 
            Shift(a);

        if(mutation>=2 and mutation<3); 
            Rotate(a);

        if(mutation>=3 and mutation<4); 
            Invert(a);

        