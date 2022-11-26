#script de leitura

for i in 50 
do
    for j in 50
    do
          for k in 1 
          do
                 for l in $(seq 5)
                 do     
                        g++ AG.cpp -o AG
                       ./AG CapEuclideo.${i}.${j}.${k}.${l}.tpp
                 done
          done
    done
done
