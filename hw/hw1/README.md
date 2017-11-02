1. ./IBM\ Quest\ Data\ Generator/gen lit -ascii -ntrans <tran_num> -tlen <tran_len> -nitems <item_num> > <ibm_file>
2. awk -f awk_IBM_data_to_csv <ibm_file> > <csv_file>
3. cd Asso
4. make clean MYDEF=-DASSO all
5. ./asso <csv_file> <min_sup> <min_conf>
