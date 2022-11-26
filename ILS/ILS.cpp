#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define maxn 350

using namespace std;

double **distancia;//distância entre os vértices do grafo
int *demanda;  //demanda de cada produto que deve ser atendida
int **quantidade;//quantidade do produto 'k' disponível no mercado 'j'
int **valor; //custo do produto 'k' no mercado 'j'
int n;//número de vértices do grafo
int m;//número de produtos
int solucao[maxn];
int original[maxn][maxn];
int n2;
int custo;
int vai[maxn];
double customelhor=10000000;//Armazena o custo da melhor solução encontrada
int numerodemercados;
int configmelhor[maxn];//Armazena a configuração de visita da melhor solução encontrada




int Alee(int min,int max){
    return rand()%(max-min+1) + min;
}


int numero(string num){
    stringstream ss(num);
    int retorno=0;
    ss >> retorno;
    return retorno;
}


void leitura(char *nome){
	string ler;
	ifstream arquivo(nome);
	arquivo>>ler;

	n=numero(ler);
	int cordx[n];
	int cordy[n];
	
	for(int i=0;i<n;i++){
		arquivo>>ler;
		arquivo>>ler;
		cordx[i]=numero(ler);
		arquivo>>ler;
		cordy[i]=numero(ler);
	}
    distancia=new double*[n];
	for(int i=0;i<n;i++){
		distancia[i]=new double[n];
		for(int j=0;j<n;j++){
			distancia[i][j]=sqrt(pow(cordx[j]-cordx[i],2) + pow(cordy[j]-cordy[i],2));
		}
	}

	arquivo>>ler;

	m=numero(ler);
	demanda=new int[m];
	for(int i=0;i<m;i++){
		arquivo>>ler;
		arquivo>>ler;
		demanda[i]=numero(ler);
	}
	arquivo>>ler;
	arquivo>>ler;
	quantidade=new int*[m];
	valor=new int*[m];
	for(int k=0;k<m;k++){
		quantidade[k]=new int[n];
		valor[k]=new int[n];
	}
	for(int j=1 ; j<n; j++){
		arquivo>>ler;
		arquivo>>ler;
		int tam=numero(ler);
		for(int k=0;k<tam;k++){
			arquivo>>ler;
			int indice=numero(ler);
			arquivo>>ler;
			valor[indice-1][j]=numero(ler);
			arquivo>>ler;
			quantidade[indice-1][j]=numero(ler);
		}
	}

	arquivo.close();

}

int fh(int solucao[],int k){
    int total=0;
    total+=original[solucao[k-1]][0] + original[0][solucao[1]];
    for(int i=1;i<k-2;i++){
        total+=original[solucao[i]][solucao[i+1]];
    }
    return total;
}


void pertubacao(){
	int sol_inici[n-1];
	int demandaaux[m];
	int quantidadeaux[n][m];

	vector<int> mercados;

	for(int i=1;i<n;i++){
		mercados.push_back(i);
	}

	for(int i=1;i<n;i++){
		int ind=Alee(0,mercados.size()-1);
        sol_inici[i-1]=mercados[ind];
		for(int k=0;k<m;k++){
			if(quantidadeaux[k][i]>0){
				if(demandaaux[k]>0){
					if(demandaaux[k]<=quantidadeaux[k][i]){
						quantidadeaux[k][i]=quantidadeaux[k][i]-demandaaux[k];
						demandaaux[k]=0;
					}
					else{
						demandaaux[k]=demandaaux[k]-quantidadeaux[k][i];
						quantidadeaux[k][i]=0;
					}
				}
			}
		}
		mercados[ind]=mercados[mercados.size()-1];
        mercados.pop_back();
        int teste=0;
        for(int l=0;l<m;l++){
            if(demandaaux[l]>0){
                teste=1;
            }
        }
        if(teste==0){
            break;
        }
	}
    solucao[0]=0;
	for(int i=0;i<n-1;i++){
		solucao[i+1]=sol_inici[i];
	}
}

void permuta(){
	int sol[n2];
	int custoatual=1000000;
	for(int i=0;i<n2;i++){
		sol[i]=solucao[i+1];
	}
    for(int j=0;j<n2;j++){
    	int ind=Alee(0,n2-1);
    	if(ind==j){
    		ind=Alee(0,n2-1);
    	}
    	int aux=sol[j];
    	sol[j]=sol[ind];
    	sol[ind]=aux;

    	custo=fh(sol,n2);
    	if(custo<custoatual){
            custoatual=custo;
            for(int i=0;i<n2;i++){
            	solucao[i+1]=sol[i];
            }
        }
    }
    custo=custoatual;

}

void busca_local(){
	int custoatual=1000000;
	for(int i=1;i<=n;i++){
		permuta();
		if(custo<custoatual){
			custoatual=custo;
		}
	}
	custo=custoatual;
	
}


void otimizacao(){
    int compras[m];
    int quantidadeaux[n][m];
    int valoraux[n][m];
    bool visitei[n];
    for(int j=0;j<n;j++){
        visitei[j]=false;
        for(int k=0;k<m;k++){
            quantidadeaux[k][j]=quantidade[k][j];
            valoraux[k][j]=valor[k][j];
        }
    }
    int notafiscal=0;
    for(int i=0;i<m;i++){
        compras[i]=demanda[i];
    }
    for(int pro=0;pro<m;pro++){
        int instante=1;
        while(compras[pro]>0){
            if(instante==n+1){
                notafiscal+=1000000;
                    break;
            }
            instante++;
            int maisbarato=1000000;
            int local;
            for(int mer=1;mer<n;mer++){
                if(quantidadeaux[pro][solucao[mer]]>0){
                    if(valoraux[pro][solucao[mer]]<maisbarato){
                        maisbarato=valoraux[pro][solucao[mer]];
                        local=solucao[mer];
                    }
                }
            }
            if(quantidadeaux[pro][local]>=compras[pro]){
                    quantidadeaux[pro][local]-=compras[pro];
                    notafiscal+=compras[pro]*valoraux[pro][local];
                    compras[pro]=0;
            }
            else{
                compras[pro]-=quantidadeaux[pro][local];
                notafiscal+=quantidadeaux[pro][local]*valoraux[pro][local];
                quantidadeaux[pro][local]=0;
            }
            visitei[local]=true;
        }
    }
    int conta=0;
    for(int vis=1;vis<n;vis++){
        conta++;
        if(conta==n){
            break;
        }
        if(solucao[vis]==0){
                break;
        }
        if(visitei[solucao[vis]]==false){
            for(int vis2=vis;vis2<n;vis2++){
                solucao[vis2]=solucao[vis2+1];
            }
            vis--;
        }
    }
    solucao[0]=notafiscal;
    int quantmercados=0;
    for(int me=1;me<n;me++){
        if(solucao[me]>0){
            quantmercados++;
        }
    }
    n2=quantmercados;
    for(int pontos=1;pontos<quantmercados;pontos++){
        for(int pontos2=1;pontos2<quantmercados;pontos2++){
            original[pontos-1][pontos2-1]=distancia[solucao[pontos]][solucao[pontos2]];
        }
    }
    busca_local();
    solucao[0]+=custo;
    if(solucao[0]<customelhor){
        customelhor=solucao[0];
        numerodemercados=n2;
        for(int ji=1;ji<n2;ji++){
            configmelhor[ji]=solucao[ji];
        }
        for(int ji=n2+1;ji<n;ji++){
            configmelhor[ji]=0;
        }
    }
    
}

double tempo;//Variável que calcula o tempo do AG

void ILS(){
    double inicio=clock();
    int k=0;
    int sol_atual=1000000;
	pertubacao();
	otimizacao();
    while(k<pow(n,2)){
        sol_atual=customelhor;
    	pertubacao();
    	otimizacao();
    	k++;
        cout<<customelhor<<endl;
    	if(((clock()- inicio)/CLOCKS_PER_SEC) == 300){
    		 break;

    	}
    }
	tempo=(clock()-inicio)/CLOCKS_PER_SEC;
  
}


int main(int argc, char *argv[]){
	srand(time(NULL));
	leitura(argv[1]);
	cout<<"instancia: "<<argv[1]<<endl;
    ILS();
    ofstream arquivo;
	arquivo.open ("resultados_ILS.txt", ofstream::out | ofstream::app);
	arquivo << argv[1];
	arquivo << " ;";
	arquivo << customelhor;
	arquivo << " ;";
	arquivo << n2+1;
	arquivo << " ;";
	arquivo << tempo;
	arquivo << "\n";	
	return 0;

}
