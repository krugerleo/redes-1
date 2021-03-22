# Relatorio trabalho de redes
Aluno: Leonardo Bueno Nogueira Kruger GRR:20180130
Relatorio relativo ao trabalho de redes, implementação de um editor de texto online.


# Files

O trabalho está dividido nos seguintes arquivos
**client.c**, **server.c**, **communication.c**, **client.h**, **server.h**, **communication.h** 

## Exemplo de Utilização
Em um terminal:
```
root@kruger-Ubuntu:/home/kruger/Clones/redes-1# make
gcc client.c commands.c communication.c -o client -Wall -Wextra -pedantic -std=c99
gcc server.c commands.c communication.c -o server -Wall -Wextra -pedantic -std=c99
root@kruger-Ubuntu:/home/kruger/Clones/redes-1#  ./server
```
Em outro terminal:
```
root@kruger-Ubuntu:/home/kruger/Clones/redes-1#  ./client
escreva um comando
cd ..
	| Sucesso
```
Comandos:
```
1. cd <nome_dir> – efetua a troca de diretório no servidor
2. lcd <nome-dir> – efetua a troca de diretório no cliente
3. ls – lista os arquivos do diretório corrente do servidor
4. lls – lista os arquivos do diretório corrente do cliente
5. ver <nome_arq> - mostra o conteúdo do arquivo texto do servidor na tela do cliente. As
linhas do arquivo devem ser numeradas para mostrar na tela.
6. linha <numero_linha> <nome_arq> - mostra a linha <numero_linha> do arquivo
<nome_arq> que esta no servidor na tela do cliente.
7. linhas <numero_linha_inicial> <numero_linha_final> <nome_arq> - mostra as linhas
entre a <numero_linha_inicial> e <numero_linha_final> do arquivo <nome_arq> que
esta no servidor na tela do cliente.
8. edit <numero_linha> <nome_arq> “<NOVO_TEXTO>” – troca a linha <numero_linha> do
arquivo <nome_arq> que esta no servidor pelo texto <NOVO_TEXTO> que deve ser
digitado entre aspas. As aspas são delimitadores do texto e não devem ser escritas no
arquivo.
```
## Definições

Algumas definições utilizadas no trabalho conforme especificações:
```
// tipo da mensagem
 typedef  enum {
  CDT = 0  // 0000 0
, LST  // 0001 1
, VERT  // 0010 2
, LINHAT  // 0011 3
, LINHAST  // 0100 4
, EDITT  // 0101 5
, NOTSET  // 0110 6
, NOTSET2  // 0111 7
, ACK  // 1000 8
, NACK  // 1001 9
, LIF  // 1010 10
, CONTLS  // 1011 11
, CONTARQ  // 1100 12
, EOT  // 1101 13
, NOTSET3  // 1110 14
, ERROR  // 1111 15
} MSG_TYPE;

#define  MARK  126 // 01111110 Marcador de início 

// Remetente e destinatario
typedef  struct  Sender{
unsigned  char  dest : 2;
unsigned  char  orig : 2;
} Sender;

// Cabeçalho para o protocol kermit
typedef  struct  Package_header
{
unsigned  char  mark : 8; // 01111110
unsigned  char  dest : 2;
unsigned  char  orig : 2;
unsigned  char  size : 4;
unsigned  char  seq : 4;
unsigned  char  type : 4;
unsigned  char  data[15];
unsigned  char  par;
} Package_header;
```

## Funções destaque

```void  send_msgs(char *data, int  type, int  orig, int  dest);```
Envia uma mensagem que não necessita de resposta.
```void  send_last();```
Reenvia ultima mensagem.
```void  communication(char *data, int  type, int  orig, int  dest, int  first);```
Inicia uma comunicação com o destinatario, fragmentando o data em pedaços [15] se necessario e enviando
uma mensagem por vez após o recebimento do ACK;
A variavel first sincroniza a seq = 0 para inicio de uma nova comunicação.
```void init_socket();```
Inicia o socket sock_r universal para ser utilizado no envio e recebimento de mensagens.
```Package_header * recieve_msg();```
Recebe a mensagem;


## Problemas e dificuldade

Pontos faltantes do trabalho: **timeout** tive dificuldades com a implementação do timeout, devido a ter ficado por ultimo creio que o desenvolvimento do jeito que foi feito dificultou a junção dessa função e o trabalho acabou ficando com problemas de espera infinita.


# Considerações finais

O trabalho proporcionou uma visão pratica da implementação de transmissão de socket e do protocolo kermit, a falta de conteudo sobre implementação não era abundante e dificultou um pouco porém o tempo para realização do trabalho compensou.
Demandou muita pesquisa e consequentemente a leitura sobre aplicação dos sockets com outros protocolos, no geral demandou bastante esforço porém recompensa quando você vê os resultados.

