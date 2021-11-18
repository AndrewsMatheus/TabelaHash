#include <cstdio>
#include <cctype>
#include <cstring>
#include <iostream>

#define MIN 3
#define MAX 20


/*
  Lê um texto de um arquivo passado como primeiro parâmetro e cria um
  contador de ocorrências para palavras com pelo menos três letras,
  utilizando uma tabela de dispersão.
*/


// Célula de uma lista encadeada de palavras, armazena uma cadeia de caracteres 
class Palavra {
  friend class ListaP;

public:
  Palavra(char p[]);      // Construtor padrão
  ~Palavra();             // Destrutor padrão
  void incrementa();      // Conta mais uma ocorrência da palavra no texto
  void escreve();         // Escreve a palavra e sua quantidade de ocorrências

private:
  char p[MAX+1];      // A palavra em si
  int ocorrencias;    // Contador de ocorrências
  Palavra *prox;      // Próxima Palavra na lista
};
Palavra::Palavra(char p[]){

  this->ocorrencias= 1;
  strcpy(this->p, p);
  this->prox= nullptr;
}
Palavra::~Palavra(){
  
}
void Palavra::escreve(){
  std::cout << p<<"(" << ocorrencias <<") ";
}
void Palavra::incrementa(){
  ocorrencias++;
}
// Lista encadeada de palavras, inserção realizada no fim da lista.
// Tenha o cuidado de não inserir duplicatas 

class ListaP {
public:
  ListaP();                  // Construtor padrão
  ~ListaP();                 // Destrutor padrão
  Palavra *insere(char p[]); // Recebe uma palavra e insere na lista, devolvendo um ponteiro para o objeto inserido
  Palavra *busca(char p[]);  // Busca e devolve o objeto de uma palavra na lista
  int tamanho();             // Devolve o tamanho da lista
  void escreve();            // Escreve os elementos da lista

private:
  Palavra *inicio; // Início da lista
  Palavra *fim;    // Fim da lista
};

ListaP::ListaP(){

  this->inicio= nullptr;
  this->fim= nullptr;

}
Palavra* ListaP::insere(char p[]){

  Palavra* novo= new Palavra(p);

  if(inicio == nullptr){
    
    inicio= fim= novo;
    return fim;
  }
  else{

    fim->prox= novo;
    fim= fim->prox;
    return fim;
  }
}
Palavra* ListaP::busca(char p[]){

    for(Palavra* aux= inicio; aux != 0; aux= aux->prox){
      if(strcmp(aux->p, p)==0)
        return aux;
    }
    
    return nullptr;
}
int ListaP::tamanho(){

  int tamanho = 0;

  for(Palavra* aux= inicio; aux != nullptr; aux= aux->prox)
    tamanho++;

  return tamanho;
}
void ListaP::escreve(){
  
  for(Palavra* aux= inicio; aux != 0; aux= aux->prox){
    aux->escreve();
  }
    std::cout << std::endl;
}
ListaP::~ListaP(){
  Palavra *p;
  Palavra *aux;

  p = inicio;

  while(p){
    aux = p;
    p = p->prox;
    delete aux;
  }
}
// Tabela de dispersão que guarda um conjunto de palavras com seus contadores
class TabelaDispersao {
public:
  TabelaDispersao();  // Construtor padrão
  ~TabelaDispersao(); // Destrutor padrão
  void escreve();     // Escreve a tabela (incluindo fator de carga)
  double fator();     // Calcula e devolve o fator de carga
  void novaOcorrencia(char p[]); // Registra uma nova ocorrência de uma palavra
  void tamanho();
private:
  static const int TAMANHO = 47; // Tamanho da tabela
  ListaP tabela[TAMANHO];        // Listas da tabela

  int valor(char p[]); // Valor da palavra p
  int h(char p[]);     // Função de hash
};
TabelaDispersao::TabelaDispersao(){
  
}
void TabelaDispersao::escreve(){
  
  for(int i= 0; i<TAMANHO; i++){
    std::cout << i<< ": ";
      tabela[i].escreve();
      
  }
}
double TabelaDispersao::fator(){
  int n= 0;

  for(int i=0; i<TAMANHO; i++)
    n+= tabela[i].tamanho();
  

  return (double)n/(double)TAMANHO;
}
void TabelaDispersao::novaOcorrencia(char p[]){
  int ind= h(p);

  Palavra* ptr= tabela[ind].busca(p);

  if(ptr != nullptr)
    ptr->incrementa();
  else
    tabela[ind].insere(p);
}
int TabelaDispersao::valor(char p[]){

  int i=0;
  int valor=0;

  while(p[i] != '\0'){

    valor+= (int) p[i];
    i++;
  }

  return valor;
}
int TabelaDispersao::h(char p[]){
  return valor(p) % TAMANHO;
}
TabelaDispersao::~TabelaDispersao(){
}

int main(int argc, char *argv[]){
  
  FILE *arq;
  char palavra[MAX];
  char cpy[MAX];
  char aux;
  int tamanhoPalavra, i;

  TabelaDispersao *hash= new TabelaDispersao();

  arq = fopen(argv[1], "r");

  if(arq != NULL){
      fscanf(arq, "%s", palavra);
    while(!feof(arq)){

      tamanhoPalavra= strlen(palavra);

      if(tamanhoPalavra >= MIN && tamanhoPalavra <= MAX){
        i=0;
        while(palavra[i] && i != -1){

          if((palavra[i] >= 'a' && palavra[i] <= 'z') || (palavra[i] >= 'A' && palavra[i] <= 'Z')){
            aux= palavra[i];
            palavra[i]= tolower(aux);
            i++;  
            }
          else{

            if(i >= MIN){
              strncpy(cpy, palavra, i);
              cpy[i]= '\0';
              hash->novaOcorrencia(cpy);
            }
              i=-1;
          }
        }
        if(i != -1){
          hash->novaOcorrencia(palavra);
        }
      }
      fscanf(arq, "%s", palavra);
    }
  }
  else
    std::cout << "Arquivo invalido!!"<< std::endl;
  
  hash->escreve();
  std::cout << "Fator de carga: "<< hash->fator();
  fclose(arq);
  delete hash;
  return 0;
}
