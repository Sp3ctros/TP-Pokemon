#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LEN 100
#define Max_num_pokemons 100
#define Bonus_ataque 1.2f
#define Debuff_ataque 0.8f




// tipos de pokemons como constantes para o calculo de batalha
enum TipoPokemon { ELETRICO = 1, AGUA = 2, FOGO = 3, GELO = 4, PEDRA = 5};

// Estrutura de um Pokémon
struct pokemon
{
    char nome[MAX_STRING_LEN];
    char tipo[MAX_STRING_LEN];
    int ataque;
    int defesa;
    float vida;
};

// EStrutura de um treinador
typedef struct
{
    struct pokemon cartel_pokemon[Max_num_pokemons];
    int num_pokemons;
    int num_pokemons_vivos;
} treinador;



// converter o tipo de pokemon para valor numérico
int converterTipo(char*tipo)
{
    switch(tolower(tipo[0]))
    {
        case 'a':
            return AGUA;
        case 'e':
            return ELETRICO;
        case 'f':
            return FOGO;
        case 'g':
            return GELO;
        case 'p':
            return PEDRA;
        default:
            return 0; // Erro! Pokemon nao pertence a nenhum tipo pré estabelecido.
    }
}

// Função de batalha entre treinadores
int pokebatalha(treinador *atacante, int index_A, treinador *defensor, int index_B)
{   int tipo_atacante = converterTipo(atacante->cartel_pokemon[index_A].tipo);
    int tipo_defensor = converterTipo(defensor->cartel_pokemon[index_B].tipo);
    if (tipo_atacante == 0 || tipo_defensor == 0)
    {
        printf("\033[H\033[J");
        printf("Erro! Pokemon nao pertence a nenhum tipo pre estabelecido.\n");
        return 1; //erro no tipo
    }
    int power_relation = converterTipo(atacante->cartel_pokemon[index_A].tipo) - converterTipo(defensor->cartel_pokemon[index_B].tipo); 
    float ataque_atacante = atacante->cartel_pokemon[index_A].ataque;
    float defesa_defensor = defensor->cartel_pokemon[index_B].defesa;
    switch (power_relation)
    {
    case -1:
        ataque_atacante *= Bonus_ataque;
        break;
    case 1:
        ataque_atacante *= Debuff_ataque;
        break;
    case 4:
        ataque_atacante *= Bonus_ataque;
        break;
    case -4:
        ataque_atacante *= Debuff_ataque;
    default:
        break;
    }

    if (ataque_atacante > defesa_defensor)
    {
        defensor->cartel_pokemon[index_B].vida -= (ataque_atacante - defesa_defensor);
    }
    else
    {
        defensor->cartel_pokemon[index_B].vida--;
    }

    if (defensor->cartel_pokemon[index_B].vida <= 0)
    {
        defensor->num_pokemons_vivos--;
        printf("%s venceu %s\n", atacante->cartel_pokemon[index_A].nome, defensor->cartel_pokemon[index_B].nome);
        if (defensor->num_pokemons_vivos <= 0)
        {
            return 0;
        }
        index_B++;
        
    }
    pokebatalha(defensor, index_B, atacante, index_A);
    return 0;
}


// Função para ler pokémons de um arquivo e separar no vetor treinador A e B
int lerPokemons(FILE *arq, treinador *treinadorA, treinador *treinadorB) 
{
    char linha_buffer[MAX_STRING_LEN];
    fscanf(arq, "%d %d\n", &treinadorA->num_pokemons, &treinadorB->num_pokemons); // Pega o numero de pokemons de cada treinador
    if (treinadorA->num_pokemons < 1 || treinadorB->num_pokemons < 1)
    {
        printf("Num pokemons:\nTreinador A: %d\nTreinador B: %d\n", treinadorA->num_pokemons, treinadorB->num_pokemons);
        return 1;
    }
    else if (treinadorA->num_pokemons > 100 || treinadorB->num_pokemons > 100)
    {
        printf("Num pokemons:\nTreinador A: %d\nTreinador B: %d\n", treinadorA->num_pokemons, treinadorB->num_pokemons);
        return 2;
    }
    treinadorA->num_pokemons_vivos = treinadorA->num_pokemons;
    treinadorB->num_pokemons_vivos = treinadorB->num_pokemons;
    for (int i = 0; i < treinadorA->num_pokemons + treinadorB->num_pokemons; i++) // separa os dados de cada pokemon
    {
        fgets(linha_buffer, MAX_STRING_LEN, arq);
        if (i < treinadorA->num_pokemons) 
        { 
            //separa os dados do pokemon e verifica se ele tem 5 caracteristicas.
            if (sscanf(linha_buffer, "%s %d %d %f %s", treinadorA->cartel_pokemon[i].nome, &treinadorA->cartel_pokemon[i].ataque, &treinadorA->cartel_pokemon[i].defesa, &treinadorA->cartel_pokemon[i].vida, treinadorA->cartel_pokemon[i].tipo) != 5)
            {
                printf("%s", linha_buffer);
                return 3;
            }
        } 
        else 
        {
            if (sscanf(linha_buffer, "%s %d %d %f %s", treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].nome, &treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].ataque, &treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].defesa, &treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].vida, treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].tipo) != 5)
            {
                printf("%s", linha_buffer);
                return 3;
            }
        }
    }
    return 0;
}

// Mostra todos pokemons que nao foram derrotados
void mostrar_sobreviventes(treinador *treinadorA, treinador *treinadorB)
{
    printf("Pokemons sobreviventes: \n");
    for(int i = 0; i < treinadorA->num_pokemons + treinadorB->num_pokemons; i++)
    {
        if (i < treinadorA->num_pokemons)
        {
            if (treinadorA->cartel_pokemon[i].vida > 0)
             {
                printf("%s\n", treinadorA->cartel_pokemon[i].nome);
             }
        }
        else
        {
            if (treinadorB->cartel_pokemon[i-treinadorA->num_pokemons].vida > 0)
            {
                printf("%s\n", treinadorB->cartel_pokemon[i-treinadorA->num_pokemons].nome);
            }
        }
    }
}

// Mostra todos pokemons que foram derrotados
void mostrar_derrotados(treinador *treinadorA, treinador *treinadorB)
{
    printf("Pokemons derrotados: \n");
    for(int i = 0; i < treinadorA->num_pokemons + treinadorB->num_pokemons; i++)
    {
        if (i < treinadorA->num_pokemons)
        {
            if (treinadorA->cartel_pokemon[i].vida <= 0)
             {
                printf("%s\n", treinadorA->cartel_pokemon[i].nome);
             }
        }
        else
        {
            if (treinadorB->cartel_pokemon[i-treinadorA->num_pokemons].vida <= 0)
            {
                printf("%s\n", treinadorB->cartel_pokemon[i-treinadorA->num_pokemons].nome);
            }
        }
    }
}

void mostrar_dados(treinador *treinadorA, treinador *treinadorB)
{
    printf("------------------------------------------------ DADOS DOS POKEMONS -----------------------------------------------------------------------------------------------------------\n");
    printf("%40s %60s\n", "Treinador A","Treinador B");
    printf("%40s:%4d %60s:%4d\n", "Numeros de pokemons", treinadorA->num_pokemons, "Numeros de pokemons",treinadorB->num_pokemons);
    printf("%-20s %-6s %-6s %-6s %38s %-20s %-6s %-6s %-6s\n", 
           "Nome", "Ataque", "Defesa", "Vida", "", "Nome", "Ataque", "Defesa", "Vida");

    int max_pokemons = (treinadorA->num_pokemons > treinadorB->num_pokemons) ? treinadorA->num_pokemons  : treinadorB->num_pokemons;

    for (int i = 0; i < max_pokemons; i++)
    {
        if (i < treinadorA->num_pokemons)
        {
            printf("%-20s %-6d %-6d %-6.0f", 
                   treinadorA->cartel_pokemon[i].nome,
                   treinadorA->cartel_pokemon[i].ataque, 
                   treinadorA->cartel_pokemon[i].defesa,
                   treinadorA->cartel_pokemon[i].vida);
        }
        else
        {
            printf("%-20s %-6s %-6s %-6s", "", "", "", "");
        }

        printf("%40s", "");

        if (i < treinadorB->num_pokemons)
        {
            printf("%-20s %-6d %-6d %-6.0f", 
                   treinadorB->cartel_pokemon[i].nome, 
                   treinadorB->cartel_pokemon[i].ataque, 
                   treinadorB->cartel_pokemon[i].defesa,
                   treinadorB->cartel_pokemon[i].vida);
        }

        printf("\n");
    }
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}


int main() 
{
    treinador treinadorA, treinadorB;
    FILE *arq; 
    arq = fopen("text.txt", "r"); 
    if (arq == NULL) 
    {
        printf("Erro ao abrir arquivo!\n");
        return 1; 
    }
    //separa os dados dos pokemons e dos treinadores e verifica os cenarios de erro
    switch (lerPokemons(arq, &treinadorA, &treinadorB))
    {
    case 1:
        printf("Erro! Os Treinadores precisam ter no minimo 1 pokemon!\n");
        return 2;
    case 2:
        printf("Erro! Os Treinadores precisam ter no maximo 100 pokemons!\n");
        return 3;
    case 3:
        printf("Erro! Os pokemons precisam ter 5 caracteristicas (Nome, ataque, defesa, vida e tipo)\n");
        return 4;
    default:
        break;
        
    }
    int pok_A_index = 0, pok_B_index = 0;
    //BATALHAAAA!
    mostrar_dados(&treinadorA, &treinadorB);
    if (pokebatalha(&treinadorA, pok_A_index, &treinadorB, pok_B_index) == 1) return 4;
    printf(treinadorA.num_pokemons_vivos == 0 ? "Jogador 2 venceu\n": "Jogador 1 venceu\n");
    mostrar_sobreviventes(&treinadorA, &treinadorB);
    mostrar_derrotados(&treinadorA, &treinadorB);
    printf("\n");
    return 0;
}
