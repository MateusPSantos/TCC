#include <ilcplex/ilocplex.h>
#include <ilcplex/ilocplex.h>
#include <iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<time.h>
#include <math.h>
#define maxn 350
#define maxm 350
using namespace std;

int n;
int m;
int localx[maxn];
int localy[maxn];
double distancia[maxn][maxn];
int demanda[maxm];


int numero(string num){
    stringstream ss(num);
    int retorno=0;
    ss >> retorno;
    return retorno;
}
struct mercado{// Um registro de um mercado guarda seus produtos à venda
               // e seus respectivos custos
    int quantidade[maxm];
    int valor[maxm];
};
mercado mercados[maxn];

void leitura(char *nome){
	string aux;
	ifstream arq(nome);
    arq>>aux;
    n=numero(aux);
    for(int i=1;i<=n;i++){
    	arq>>aux;
    	arq>>aux;
    	localx[i]=numero(aux);
    	arq>>aux;
    	localy[i]=numero(aux);
    }
    for(int i=1;i<=n;i++){
    	for(int j=1;j<=n;j++){
    		distancia[i][j]=sqrt(pow(localx[j]-localx[i],2)+pow(localy[j]-localy[i],2));
    	}
    }
    arq>>aux;
    m=numero(aux);
    for(int i=1;i<=m;i++){
    	arq>>aux;
    	arq>>aux;
    	demanda[i]=numero(aux);
    }
    for(int i=1;i<=n;i++){
        arq>>aux;
        arq>>aux;
        int quant=numero(aux);
        for(int j=1;j<=quant;j++){
            arq>>aux;
            int prod=numero(aux);
            arq>>aux;
            mercados[i].valor[prod]=numero(aux);
            arq>>aux;
            mercados[i].quantidade[prod]=numero(aux);
        }
    }
    arq.close();
}






ILOSTLBEGIN
int main(int argc, char *argv[]){
    leitura(argv[1]);

    IloEnv env;
    try{
       IloModel model(env);

        IloArray<IloIntVarArray>x(env,n);
		IloNumVarArray y(env,n,0,1);
		IloArray<IloIntVarArray>z(env,m);

		for(int i=1;i<=n;i++){
		    x[i]=IloIntVarArray(env,n,0,1);
		}

		for(int k=1;k<=m;k++){
		   z[k]=IloIntVarArray(env,n,0,IloInfinity);
		}

		IloExpr funcaoobjetivo(env);

		for(int i=1;i<=n;i++){
		    for(int j=1;j<=n;j++){
		        funcaoobjetivo+=distancia[i][j]*x[i][j];
            }
		}
		for(int k=1;k<=m;k++){
		    for(int j=1;j<=n;j++){
		        funcaoobjetivo+=mercados[j].valor[k]*z[k][j];
		    }
		}
		model.add(IloMinimize(env,funcaoobjetivo));

		for(int i=1;i<=n;i++){
		   IloExpr c1;
		   for(int j=i+1;j<=n;j++){
		       c1+=x[i][j];
		   }
		   model.add(c1==2*y[i]);
		}

		for(int i=1;i<=n-1;i++){
		   IloExpr c2;
		   for(int j=i+1;j<=n;j++){
		       c2+=x[i][j];
		   }
		   model.add(c2>=2*y[i]);
		}

		for(int k=1;k<=m;k++){
		   IloExpr c3;
		   for(int j=1;j<=n;j++){
		       c3+=z[k][j];
		   }
		   model.add(c3==demanda[k]);
		}

		for(int k=1;k<=m;k++){
		    IloExpr c4;
		    for(int j=1;j<=n;j++){
		        c4=z[k][j];
		        model.add(c4<=y[j]*mercados[j].quantidade[k]);
		    }
		}

		IloCplex cplex(model);
		cplex.solve();
		cout << "\n\nCplex success!\n";
        cout << "\tStatus: " << cplex.getStatus() <<endl;
        cout << "\tObjective value: " << cplex.getObjValue() <<endl;

    }catch (IloException& ex) {
    env.out() << "Error: " << ex << std::endl;
  }
  env.end();

}
