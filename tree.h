#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef struct tipoNo {
  long long matricula;
  char nome[100];
  struct tipoNo *pai;
  struct tipoNo *esq;
  struct tipoNo *dir;
} TreeNode;

void iniciaArvore(TreeNode **arvore);
TreeNode *criar_no(long long matricula, const char *nome);
long long entradaPerguntaUsuario();
TreeNode *inserir(TreeNode **arvore, long long matricula, const char *nome);
void carregarDadosDoArquivo(TreeNode **arvore);
TreeNode *buscar(TreeNode *arvore, long long matricula);
TreeNode *encontrar_minimo(TreeNode *no);
void transplantar(TreeNode **arvore, TreeNode *alvo, TreeNode *novo_no);
void validarArvore(TreeNode *no);
void imprimir_em_ordem(TreeNode *no);
void imprimir_pre_ordem(TreeNode *no);
void imprimir_pos_ordem(TreeNode *no);
void percorreArv(TreeNode *no, FILE *dado);
void salvarArq(TreeNode *arvore);
void menu(TreeNode **arvore);
void imprimir_menu();
void imprimirResultadoBusca(TreeNode *no, long long matricula_buscar);
void liberar_nos(TreeNode *no);

#endif
