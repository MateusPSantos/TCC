#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include<vector>
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#define maxn 300
#define maxm 300
#define pop 100

using namespace std;


//Digite o nome da instância que deseja Solucionar




int customelhor=10000000;//Armazena o custo da melhor solução encontrada
int configmelhor[maxn];//Armazena a configuração de visita da melhor solução encontrada
int n;//Número de mercados
int m;//Número de produtos
int localx[maxn];
int localy[maxn];
int distancia[maxn][maxn];//Armazena a distância de todos os pontos para todos os outros
int demanda[maxm];//Armazena a demanda de Produtos
int individuo[pop][maxn];//Armazena a população
int novosindividuos[pop][maxn];//Armazena uma nova população gerada a partir dos operadores
                               // do AG com relação a população atual
int numero(string num){
    stringstream ss(num);
    int retorno=0;
    ss >> retorno;
    return retorno;
}

int Alee(int min,int max){
    return rand()%(max-min+1) + min;
}
struct mercado{// Um registro de um mercado guarda seus produtos à venda
               // e seus respectivos custos
    int quantidade[maxm];
    int valor[maxm];
};

mercado mercados[maxn];// Temos um registro para cada mercado relativo aos
                       // seus produtos à venda e seus respectivos custos

// Funcao de Leitura das instâncias
void leitura(char *nome){
    ifstream arq(nome);
    string aux;
    for(int i=0;i<10;i++){
        arq>>aux;
    }
    n=numero(aux);
    for(int i=0;i<8;i++){
        arq>>aux;
    }
    for(int i=1;i<=n;i++){
        arq>>aux;
        arq>>aux;
        localx[i]=numero(aux);
        arq>>aux;
        localy[i]=numero(aux);
    }
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            distancia[i][j]=floor(sqrt(((localx[i]-localx[j])*(localx[i]-localx[j]))+((localy[i]-localy[j])*(localy[i]-localy[j]))));
        }
    }
    arq>>aux;
    arq>>aux;
    arq>>aux;
    m=numero(aux);
    for(int i=1;i<=m;i++){
        arq>>aux;
        arq>>aux;
        demanda[i]=numero(aux);
    }
    arq>>aux;
    arq>>aux;
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
// Geração da População Inicial
void populacaoinicial(){
    for(int i=0;i<pop;i++){
        individuo[i][1]=1;
        int demandaaux[maxn];
        mercado mercadoaux[maxn];
        for(int j=1;j<=n;j++){
            for(int po=1;po<=m;po++){
                mercadoaux[j].quantidade[po]=mercados[j].quantidade[po];
                mercadoaux[j].valor[po]=mercados[j].valor[po];
            }
        }
        vector<int> possiveis;
        for(int j=2;j<=n;j++){
            possiveis.push_back(j);
        }
        for(int j=1;j<=m;j++){
            demandaaux[j]=demanda[j];
        }
        for(int j=2;j<=n;j++){
            int novo=Alee(0,possiveis.size()-1);
            for(int k=1;k<=j;k++){
                if(k==j){
                   individuo[i][k]=possiveis[novo];
                   break;
                }
                if(possiveis[novo]<individuo[i][k]){
                    for(int emp=j;emp>k;emp--){
                        individuo[i][emp]=individuo[i][emp-1];
                    }
                    individuo[i][k]=possiveis[novo];
                    break;
                }
            }
            for(int u=1;u<=m;u++){
                if(mercadoaux[possiveis[novo]].quantidade[u]>0){
                    if(demandaaux[u]>0){
                        if(demandaaux[u]<=mercadoaux[possiveis[novo]].quantidade[u]){
                            mercadoaux[possiveis[novo]].quantidade[u]=mercadoaux[possiveis[novo]].quantidade[u]-demandaaux[u];
                            demandaaux[u]=0;
                        }
                        else{
                            demandaaux[u]=demandaaux[u]-mercadoaux[possiveis[novo]].quantidade[u];
                            mercadoaux[possiveis[novo]].quantidade[u]=0;
                        }
                    }
                }
            }
            possiveis[novo]=possiveis[possiveis.size()-1];
            possiveis.pop_back();
            int teste=0;
            for(int l=1;l<=m;l++){
                if(demandaaux[l]>0){
                    teste=1;
                }
            }
            if(teste==0){
                break;
            }
        }
    }
}
int original[maxn][maxn];//Armazena distâncias correntes entre os individuos
//Calcula-se o Custo relativo à transporte
int fh(int solucao[],int k){
    int total=0;
    total+=original[solucao[k-1]][solucao[0]];
    for(int i=0;i<k-1;i++){
        total+=original[solucao[i]][solucao[i+1]];
    }
    return total;
}
int vai[maxn];
int n2;
int custo;
//Operador de Mutacao Baseado em uma técnica de inserção para o PCV
void  mutacao(){
    int MAXn=maxn;
    int aaa[MAXn];
    for(int i=0;i<n2;i++){
        aaa[i]=i;
    }
    int kk;
    int r;
    for(int i=0;i<n2;i++){
        kk=Alee(0,n2-1);
        r=aaa[i];
        aaa[i]=aaa[kk];
        aaa[kk]=r;
    }
    int solucao[MAXn];
    solucao[0]=aaa[0];
    solucao[1]=aaa[1];
    solucao[2]=aaa[2];
    int solucaoaux[MAXn];
    int k=4;
    int min[MAXn];
    min[0]=aaa[0];
    min[1]=aaa[1];
    min[2]=aaa[2];
    for(int i=3;i<n2;i++){
        min[i]=aaa[i];
        int mini=fh(min,k);
        int solux;
        for(int j=1;j<k;j++){
            for(int p=0;p<j;p++){
                solucaoaux[p]=solucao[p];
            }
            for(int pp=k-1;pp>j;pp--){
                solucaoaux[pp]=solucao[pp-1];
            }
            solucaoaux[j]=aaa[i];
            solux=fh(solucaoaux,k);
            if(solux<mini){
                mini=solux;
                for(int u=0;u<k;u++){
                    min[u]=solucaoaux[u];
                }
            }
        }
        for(int u=0;u<k;u++){
            solucao[u]=min[u];
            vai[u]=min[u];
        }
        k++;
    }
    custo=fh(solucao,n2);
}
int numerodemercados;
//Método de inserção Para o PCV
void insercao(){
    int custoatual=1000000;
    for(int i=0;i<=pop;i++){
        mutacao();
        if(custo<custoatual){
            custoatual=custo;
        }
    }
    custo=custoatual;
}
//Atualiza-se O custo dos individuos por meio do custo de compra e de transporte
void atualizacustos(){
    for(int p=0;p<pop;p++){
        int compras[maxn];
        mercado mercadoaux[maxn];
        bool visitei[maxn];
        for(int j=1;j<=n;j++){
            visitei[j]=false;
            for(int po=1;po<=m;po++){
                mercadoaux[j].quantidade[po]=mercados[j].quantidade[po];
                mercadoaux[j].valor[po]=mercados[j].valor[po];
            }
        }
        int notafiscal=0;
        for(int i=1;i<=m;i++){
            compras[i]=demanda[i];
        }
        for(int pro=1;pro<=m;pro++){
            int instante=1;
            while(compras[pro]>0){
                if(instante==n+2){
                    notafiscal+=100000;
                    break;
                }
                instante++;
                int maisbarato=1000000;
                int local;
                for(int mer=2;mer<=n;mer++){
                    if(mercadoaux[individuo[p][mer]].quantidade[pro]>0){
                        if(mercadoaux[individuo[p][mer]].valor[pro]<maisbarato){
                            maisbarato=mercadoaux[individuo[p][mer]].valor[pro];
                            local=individuo[p][mer];
                        }
                    }
                }
                if(mercadoaux[local].quantidade[pro]>=compras[pro]){
                    mercadoaux[local].quantidade[pro]-=compras[pro];
                    notafiscal+=compras[pro]*mercadoaux[local].valor[pro];
                    compras[pro]=0;
                }
                else{
                    compras[pro]-=mercadoaux[local].quantidade[pro];
                    notafiscal+=mercadoaux[local].quantidade[pro]*mercadoaux[local].valor[pro];
                    mercadoaux[local].quantidade[pro]=0;
                }
                visitei[local]=true;
            }
        }
        for(int vis=2;vis<=n;vis++){
            if(individuo[p][vis]==0){
                break;
            }
            if(visitei[individuo[p][vis]]==false){
                for(int vis2=vis;vis2<=n;vis2++){
                    individuo[p][vis2]=individuo[p][vis2+1];
                }
                vis--;
            }
        }
        individuo[p][0]=notafiscal;
        int quantmercados=0;
        for(int me=1;me<=n;me++){
            if(individuo[p][me]>0){
                quantmercados++;
            }
        }
        n2=quantmercados;
        for(int pontos=1;pontos<=quantmercados;pontos++){
            for(int pontos2=1;pontos2<=quantmercados;pontos2++){
                original[pontos-1][pontos2-1]=distancia[individuo[p][pontos]][individuo[p][pontos2]];
            }

        }
        insercao();
        individuo[p][0]+=custo;
        if(individuo[p][0]<=customelhor){
            customelhor=individuo[p][0];
            numerodemercados=n2;
            for(int ji=1;ji<=n2;ji++){
                configmelhor[ji]=individuo[p][vai[ji-1]+1];
            }
            for(int ji=n2+1;ji<=n;ji++){
                configmelhor[ji]=0;
            }

        }
    }
}
//Operador para realizacao do cruzamento
vector<int> uniao(vector<int> CONJ1,vector<int> CONJ2){
    vector<int> a;
    int j=0;
    int i=0;
    for(;;){
        if(j==CONJ2.size()){
            for(int k=i;k<CONJ1.size();k++){
                a.push_back(CONJ1[k]);
            }
            return a;
        }
        if(i==CONJ1.size()){
            for(int k=j;k<CONJ2.size();k++){
                a.push_back(CONJ2[k]);
            }
            return a;
        }
        if(CONJ1[i]==CONJ2[j]){
            a.push_back(CONJ1[i]);
            i++;
            j++;
        }
        else if(CONJ1[i]>CONJ2[j]){
            a.push_back(CONJ2[j]);
            j++;
        }
        else if(CONJ1[i]<CONJ2[j]){
            a.push_back(CONJ1[i]);
            i++;
        }
    }
}
//Operador de Cruzamento
void cruzamento(){
    for(int i=0;i<pop;i+=2){
        for(int in=0;in<=n;in++){
            novosindividuos[i][in]=0;
            novosindividuos[i+1][in]=0;
        }
        int pai=Alee(0,pop-1);
        int mae=Alee(0,pop-1);
        int cortepai=Alee(2,n-1);
        int cortemae=Alee(2,n-1);
        vector<int> pedacopai;
        vector<int> pedacomae;
        for(int co=1;co<=cortepai;co++){
            if(individuo[pai][co]==0){
                break;
            }
            pedacopai.push_back(individuo[pai][co]);
        }
        for(int co=cortemae+1;co<=n;co++){
            if(individuo[mae][co]==0){
                break;
            }
            pedacomae.push_back(individuo[mae][co]);
        }
        vector<int> uni=uniao(pedacomae,pedacopai);
        for(int loc=0;loc<uni.size();loc++){
            novosindividuos[i][loc+1]=uni[loc];
        }
        pedacomae.clear();
        pedacopai.clear();
        for(int co=cortepai+1;co<=n;co++){
            if(individuo[pai][co]==0){
                break;
            }
            pedacopai.push_back(individuo[pai][co]);
        }
        for(int co=1;co<=cortemae;co++){
            if(individuo[mae][co]==0){
                break;
            }
            pedacomae.push_back(individuo[mae][co]);
        }
        vector<int> uni2=uniao(pedacomae,pedacopai);
        for(int loc=0;loc<uni2.size();loc++){
            novosindividuos[i+1][loc+1]=uni2[loc];
        }
    }
}
//Operador de Adaptação para novos individuos não aptos após
// o cruzamento dos pais
void adaptacao(){
    for(int i=0;i<pop;i++){
        int demandaaux[maxn];
        mercado mercadoaux[maxn];
        bool esta[maxn];
        for(int j=1;j<=n;j++){
            esta[j]=false;
            for(int po=1;po<=m;po++){
                mercadoaux[j].quantidade[po]=mercados[j].quantidade[po];
                mercadoaux[j].valor[po]=mercados[j].valor[po];
            }
        }
        for(int j=1;j<=m;j++){
            demandaaux[j]=demanda[j];
        }
        vector<int> possiveis;
        esta[1]=true;
        int esk=1;
        for(int es=2;es<=n;es++){
            if(novosindividuos[i][es]==0){
                break;
            }
            esta[novosindividuos[i][es]]=true;
            esk++;
            for(int u=1;u<=m;u++){
                if(mercadoaux[novosindividuos[i][es]].quantidade[u]>0){
                    if(demandaaux[u]>0){
                        if(demandaaux[u]<=mercadoaux[novosindividuos[i][es]].quantidade[u]){
                            mercadoaux[novosindividuos[i][es]].quantidade[u]=mercadoaux[novosindividuos[i][es]].quantidade[u]-demandaaux[u];
                            demandaaux[u]=0;
                        }
                        else{
                            demandaaux[u]=demandaaux[u]-mercadoaux[novosindividuos[i][es]].quantidade[u];
                            mercadoaux[novosindividuos[i][es]].quantidade[u]=0;
                        }
                    }
                }
            }
        }
        for(int j=2;j<=n;j++){
            if(esta[j]==false){
                possiveis.push_back(j);
            }
        }
        for(int j=esk+1;j<=n;j++){
            if(possiveis.size()==0){
                break;
            }
            int novo=Alee(0,possiveis.size()-1);
            //individuo[i][j]=possiveis[novo];
            for(int k=1;k<=j;k++){
                if(k==j){
                   novosindividuos[i][k]=possiveis[novo];
                   break;
                }
                if(possiveis[novo]<novosindividuos[i][k]){
                    for(int emp=j;emp>k;emp--){
                        novosindividuos[i][emp]=novosindividuos[i][emp-1];
                    }
                    novosindividuos[i][k]=possiveis[novo];
                    break;
                }
            }
            for(int u=1;u<=m;u++){
                if(mercadoaux[possiveis[novo]].quantidade[u]>0){
                    if(demandaaux[u]>0){
                        if(demandaaux[u]<=mercadoaux[possiveis[novo]].quantidade[u]){
                            mercadoaux[possiveis[novo]].quantidade[u]=mercadoaux[possiveis[novo]].quantidade[u]-demandaaux[u];
                            demandaaux[u]=0;
                        }
                        else{
                            demandaaux[u]=demandaaux[u]-mercadoaux[possiveis[novo]].quantidade[u];
                            mercadoaux[possiveis[novo]].quantidade[u]=0;
                        }
                    }
                }
            }
            possiveis[novo]=possiveis[possiveis.size()-1];
            possiveis.pop_back();
            int teste=0;
            for(int l=1;l<=m;l++){
                if(demandaaux[l]>0){
                    teste=1;
                }
            }
            if(teste==0){
                break;
            }
        }
    }
}
void otimizacao(){
    for(int p=0;p<pop;p++){
        int compras[maxn];
        mercado mercadoaux[maxn];
        bool visitei[maxn];
        for(int j=1;j<=n;j++){
            visitei[j]=false;
            for(int po=1;po<=m;po++){
                mercadoaux[j].quantidade[po]=mercados[j].quantidade[po];
                mercadoaux[j].valor[po]=mercados[j].valor[po];
            }
        }
        int notafiscal=0;
        for(int i=1;i<=m;i++){
            compras[i]=demanda[i];
        }
        for(int pro=1;pro<=m;pro++){
            int instante=1;
            while(compras[pro]>0){
                if(instante==n+2){
                    notafiscal+=1000000;
                    break;
                }
                instante++;
                int maisbarato=1000000;
                int local;
                for(int mer=2;mer<=n;mer++){
                    if(mercadoaux[novosindividuos[p][mer]].quantidade[pro]>0){
                        if(mercadoaux[novosindividuos[p][mer]].valor[pro]<maisbarato){
                            maisbarato=mercadoaux[novosindividuos[p][mer]].valor[pro];
                            local=novosindividuos[p][mer];
                        }
                    }
                }
                if(mercadoaux[local].quantidade[pro]>=compras[pro]){
                    mercadoaux[local].quantidade[pro]-=compras[pro];
                    notafiscal+=compras[pro]*mercadoaux[local].valor[pro];
                    compras[pro]=0;
                }
                else{
                    compras[pro]-=mercadoaux[local].quantidade[pro];
                    notafiscal+=mercadoaux[local].quantidade[pro]*mercadoaux[local].valor[pro];
                    mercadoaux[local].quantidade[pro]=0;
                }
                visitei[local]=true;
            }
        }
        int conta=0;
        for(int vis=2;vis<=n;vis++){
            conta++;
            if(conta==n+1){
                break;
            }
            if(novosindividuos[p][vis]==0){
                break;
            }
            if(visitei[novosindividuos[p][vis]]==false){
                for(int vis2=vis;vis2<n;vis2++){
                    novosindividuos[p][vis2]=novosindividuos[p][vis2+1];
                }
                vis--;
            }
        }
        novosindividuos[p][0]=notafiscal;
        int quantmercados=0;
        for(int me=1;me<=n;me++){
            if(novosindividuos[p][me]>0){
                quantmercados++;
            }
        }
        n2=quantmercados;
        for(int pontos=1;pontos<=quantmercados;pontos++){
            for(int pontos2=1;pontos2<=quantmercados;pontos2++){
                original[pontos-1][pontos2-1]=distancia[novosindividuos[p][pontos]][novosindividuos[p][pontos2]];
            }
        }
        insercao();
        novosindividuos[p][0]+=custo;
        if(novosindividuos[p][0]<customelhor){
            customelhor=novosindividuos[p][0];
            numerodemercados=n2;
            for(int ji=1;ji<=n2;ji++){
                configmelhor[ji]=novosindividuos[p][1+vai[ji-1]];
            }
            for(int ji=n2+1;ji<=n;ji++){
                configmelhor[ji]=0;
            }
        }
    }
}
// Compara-se a população atual com a nova gerada
// Atualizamos a pupulação selecionando os melhores individuos entre as duas populações
void atualizacao(){
    for(int pp=0;pp<pop;pp++){
            int min=individuo[pp][0];
            int loc=pp;
            for(int ppp=pp+1;ppp<pop;ppp++){
                if(individuo[ppp][0]<min){
                    min=individuo[ppp][0];
                    loc=ppp;
                }
            }
            int tro;
            for(int i=0;i<=n;i++){
                tro=individuo[pp][i];
                individuo[pp][i]=individuo[loc][i];
                individuo[loc][i]=tro;
            }
    }
    for(int pp=0;pp<pop;pp++){
            int min=novosindividuos[pp][0];
            int loc=pp;
            for(int ppp=pp+1;ppp<pop;ppp++){
                if(novosindividuos[ppp][0]<min){
                    min=novosindividuos[ppp][0];
                    loc=ppp;
                }
            }
            int tro;
            for(int i=0;i<=n;i++){
                tro=novosindividuos[pp][i];
                novosindividuos[pp][i]=novosindividuos[loc][i];
                novosindividuos[loc][i]=tro;
            }
    }
    int populacaofinal[pop][maxn];
    int poo=0;
    int ini=0;
    int fin=0;
    for(;;){
        if(poo==pop){
            break;
        }
        if(individuo[ini][0]<novosindividuos[fin][0]){
            for(int i=0;i<=n;i++){
                populacaofinal[poo][i]=individuo[ini][i];
            }
            ini++;
            poo++;
        }
        else{
            for(int i=0;i<=n;i++){
                populacaofinal[poo][i]=novosindividuos[fin][i];
            }
            fin++;
            poo++;
        }
    }
    for(int p=0;p<pop;p++){
        for(int i=0;i<=n;i++){
            individuo[p][i]=populacaofinal[p][i];
        }
    }

    //cout<<individuo[0][0]<<endl;
    //if(individuo[0][0]<customelhor){
      //  customelhor=individuo[0][0];
    //}

}
double tempo;//Variável que calcula o tempo do AG
void AG(){
    double inicio=clock();
    populacaoinicial();
    atualizacustos();
    double soma=0;//Variável que o custo total da população
    double somaprox=0;//Variável que calcula o custo total da nova população
    for(int p=0;p<pop;p++){
        soma+=individuo[p][0];
    }
    for(;;){
        somaprox=0;
        cruzamento();
        adaptacao();
        otimizacao();
        atualizacao();
        for(int p=0;p<pop;p++){
            somaprox+=individuo[p][0];
        }
        if(somaprox=soma){// Se na próxima iteração do AG não obtivermos alguma melhora
            break;        // paramos!
        }
        soma=somaprox;
    }
    tempo=clock()-inicio;
    tempo=tempo/1000000;
}
void imprime(char *nome){//Função de impressão da solução e alguns aspectos do método
    /*
    cout<<"Custo = "<<customelhor<<endl;//Melhor solução encontrada
    cout<<"Numero de Mercados = "<<numerodemercados<<endl;//Número de mercados visitados nessa solução
    cout<<"Tempo = "<<tempo<<" segundos"<<endl;// Tempo  total do AG
    cout<<"Ordem da Rota : ";//Mercados que serão visitados já na ordem de visitação
    */
    
    /*
    int k=1;
    for(int i=1;i<=n;i++){
        if(configmelhor[i]==1){
            k=i;
        }
    }
    for(int i=k;i<=n;i++){
        if(configmelhor[i]!=0){
            cout<<configmelhor[i]<<" ";
        }
    }
    for(int i=1;i<k;i++){
        if(configmelhor[i]!=0){
            cout<<configmelhor[i]<<" ";
        }
    }
    cout<<"1 "<<endl;
    */
}

int main(int argc,char *argv[]){
    srand( (unsigned)time(NULL) );
    cout<<"executando instância "<<argv[1]<<endl;
    leitura(argv[1]);
    AG();
    //imprime(argv[1]);
    ofstream result;
    result.open("resultados_AG.txt", std::ofstream::out | std::ofstream::app);
    string linha;
    //linha = customelhor;
    result << argv[1];
    result << ";";
    result << customelhor;
    linha = ";";
    result << linha;
    result <<numerodemercados;
    result <<linha;
    result << tempo;
    result << "\n";
    result.close();



    return 0;
}
