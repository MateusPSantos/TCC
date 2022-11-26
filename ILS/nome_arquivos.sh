
#leitura de arquivos

for i in 50 100 150 200 250 300 350
do
     for j in 50 100 150 200
     do
           for k in 1 5 7 8 9 95 99
           do 
                 for l in $(seq 5)
                 do 
                       python3 ils.py CapEuclideo.${i}.${j}.${k}.${l}.tpp
                 done
           done
     done
done