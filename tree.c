#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void iniciaArvore(TreeNode **arvore) { *arvore = NULL; }

TreeNode *criar_no(long long matricula, const char *nome) {
  TreeNode *novoNO = (TreeNode *)malloc(sizeof(TreeNode));
  strcpy(novoNO->nome, nome);
  novoNO->matricula = matricula;
  novoNO->esq = NULL;
  novoNO->dir = NULL;
  novoNO->pai = NULL;

  return novoNO;
}

long long entradaPerguntaUsuario() {
  long long matricula;

  printf("Informe a matricula: ");
  scanf("%lld", &matricula);

  return matricula;
}

TreeNode *inserir(TreeNode **arvore, long long matricula, const char *nome) {
  if ((*arvore) == NULL) {
    (*arvore) = criar_no(matricula, nome);
  } else if (matricula < (*arvore)->matricula) {
    (*arvore)->esq = inserir(&(*arvore)->esq, matricula, nome);
    (*arvore)->esq->pai = (*arvore); // Define o pai do nó à esquerda
  } else {
    (*arvore)->dir = inserir(&(*arvore)->dir, matricula, nome);
    (*arvore)->dir->pai = (*arvore); // Define o pai do nó à direita
  }
  return (*arvore);
}

void carregarDadosDoArquivo(TreeNode **arvore) {
  long long mat;
  char nome[100];

  FILE *dado;
  dado = fopen("Lista_Aluno_Matricula_Atual.txt", "r");
  if (!dado) {
    printf("\nErro na abertura do arquivo 'Lista_Aluno_Matricula_Atual'\n");
    return;
  } else {
    while (fscanf(dado, "%lld ", &mat) == 1) {
      // Ler o nome até encontrar uma nova linha ou EOF
      if (fgets(nome, sizeof(nome), dado) != NULL) {
        // Remover o caractere de nova linha, se presente
        size_t len = strlen(nome);
        if (len > 0 && nome[len - 1] == '\n') {
          nome[len - 1] = '\0'; // Substitui a nova linha por um terminador nulo
        }
        // Converte o nome para uma string
        char *nome_str = strdup(nome);
        // Inserir na árvore usando o método inserir personalizado
        inserir(arvore, mat, nome_str);
      }
    } // while
    fclose(dado);
  } // else
}

TreeNode *buscar(TreeNode *arvore, long long matricula) {
  TreeNode *no = arvore;

  while (no) {
    if (matricula == no->matricula) {
      return no;
    } else if (matricula < no->matricula) {
      no = no->esq;
    } else {
      no = no->dir;
    }
  }
  return NULL; // Retorna NULL se a matrícula não for encontrada.
}

TreeNode *encontrar_minimo(TreeNode *no) {
  while (no->esq) {
    no = no->esq;
  }
  return no;
}

void remover(TreeNode **arvore, long long matricula) {
  TreeNode *no = buscar(*arvore, matricula);
  if (!no) {
    printf("Matricula %lld nao encontrada na arvore.\n", matricula);
    return;
  }
  if (!no->esq) {
    transplantar(arvore, no, no->dir);
  } else if (!no->dir) {
    transplantar(arvore, no, no->esq);
  } else {
    TreeNode *sucessor = encontrar_minimo(no->dir);
    if (sucessor->pai != no) {
      transplantar(arvore, sucessor, sucessor->dir);
      sucessor->dir = no->dir;
      sucessor->dir->pai = sucessor;
    }
    transplantar(arvore, no, sucessor);
    sucessor->esq = no->esq;
    sucessor->esq->pai = sucessor;
  }
  free(no);
}

void transplantar(TreeNode **arvore, TreeNode *alvo, TreeNode *novo_no) {
  if (!alvo->pai) {
    (*arvore) = novo_no;
  } else if (alvo == alvo->pai->esq) {
    alvo->pai->esq = novo_no;
  } else {
    alvo->pai->dir = novo_no;
  }
  if (novo_no) {
    novo_no->pai = alvo->pai;
  }
}

void validarArvore(TreeNode *no) {
  if (no == NULL) {
    printf("\nArvore vazia.\n");
    return;
  }
}

void imprimir_em_ordem(TreeNode *no) {
  if (no) {
    imprimir_em_ordem(no->esq);
    printf("\t Matricula: (%lld) -- Nome: %s\n", no->matricula, no->nome);
    imprimir_em_ordem(no->dir);
  }
}

void imprimir_pre_ordem(TreeNode *no) {
  if (no) {
    printf("\t Matricula: (%lld) -- Nome: %s\n", no->matricula, no->nome);
    imprimir_pre_ordem(no->esq);
    imprimir_pre_ordem(no->dir);
  }
}

void imprimir_pos_ordem(TreeNode *no) {
  if (no) {
    imprimir_pos_ordem(no->esq);
    imprimir_pos_ordem(no->dir);
    printf("\t Matricula: (%lld) -- Nome: %s\n", no->matricula, no->nome);
  }
}

void percorreArv(TreeNode *no, FILE *dado) {
  if (no != NULL) {
    fprintf(dado, "%lld\n", no->matricula); // Escreve a matrícula em uma linha
    fprintf(dado, "%s\n", no->nome); // Escreve o nome em uma linha separada
    percorreArv(no->esq, dado);
    percorreArv(no->dir, dado);
  }
}

void salvarArq(TreeNode *arvore) {
  FILE *dado;
  dado = fopen("Lista_Aluno_Matricula_Atual.txt", "w");
  if (!dado) {
    printf("\nErro na abertura do arquivo 'Lista_Aluno_Matricula_Atual'\n");
  } else {
    percorreArv(arvore, dado);
    fclose(dado);
  }
}

void menu(TreeNode **arvore) {
  long long matriculaInfo;
  int opcao;
  char nomeArquivo[] = "Lista_Aluno_Matricula_Atual.txt";

  carregarDadosDoArquivo(arvore);

  do {
    imprimir_menu();
    scanf("%d", &opcao);
    switch (opcao) {
    case 1:
      printf("\nDigite a matricula: ");
      long long matricula;
      scanf("%lld", &matricula);
      char nome[100];
      printf("Digite o nome: ");
      scanf("%s", nome);
      inserir(arvore, matricula, nome);
      break;
    case 2:
      matriculaInfo = entradaPerguntaUsuario();
      remover(arvore, matriculaInfo);
      break;
    case 3:
      matriculaInfo = entradaPerguntaUsuario();
      TreeNode *busca = buscar(*arvore, matriculaInfo);
      validarArvore(*arvore);
      imprimirResultadoBusca(busca, matriculaInfo);
      break;
    case 4:
      printf("\nSalvando dados no arquivo...\n");
      salvarArq(*arvore);
      printf("Base de dados salva com sucesso!\n");
      break;
    case 5:
      printf("\n\n---------- Arvore em ORDEM ----------\n\n");
      validarArvore(*arvore);
      imprimir_em_ordem(*arvore);
      printf("\n-----------------------------------------\n\n");
      break;
    case 6:
      printf("\n\n---------- Arvore em PRE-ORDEM ----------\n\n");
      validarArvore(*arvore);
      imprimir_pre_ordem(*arvore);
      printf("\n-----------------------------------------\n\n");
      break;
    case 7:
      printf("\n\n---------- Arvore em POS-ORDEM ----------\n\n");
      validarArvore(*arvore);
      imprimir_pos_ordem(*arvore);
      printf("\n-----------------------------------------\n\n");
      break;
    case 0:
      printf("Saindo do programa.\n");
      liberar_nos(*arvore);
      break;
    default:
      printf("Opção invalida. Tente novamente.\n");
      break;
    }
  } while (opcao != 0);
}

void imprimir_menu() {
  printf("\n---------- * MENU * ----------\n");
  printf("\t1. Inserir\n");
  printf("\t2. Remover\n");
  printf("\t3. Buscar\n");
  printf("\t4. Salvar base de dados\n");
  printf("\t5. Imprimir em ordem\n");
  printf("\t6. Imprimir pre-ordem\n");
  printf("\t7. Imprimir pos-ordem\n");
  printf("\t0. Sair\n\n");
  printf("Escolha uma opcao: ");
}

void imprimirResultadoBusca(TreeNode *no, long long matricula_buscar) {
  if (no) {
    printf("\n\tMatricula ENCONTRADA\n");
    printf("\t\t * Matricula: (%lld) - %s\n", no->matricula, no->nome);
  } else {
    printf("\n\tMatricula %lld - NAO ENCONTRADA\n", matricula_buscar);
  }
}

void liberar_nos(TreeNode *no) {
  if (no) {
    liberar_nos(no->esq);
    liberar_nos(no->dir);
    free(no);
  }
}
