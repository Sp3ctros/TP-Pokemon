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
        case 'e':
            return ELETRICO;
        case 'f':
            return FOGO;
        case 'g':
            return GELO;
        case 'p':
            return PEDRA;
        default:
            return AGUA; // Erro! Pokemon nao pertence a nenhum tipo pré estabelecido.
    }
}

// Função de batalha entre dois Pokémons (A ataca B)
void pokebatalha(struct pokemon *atacante, int index_A, struct pokemon *defensor, int index_B)
{
    int power_relation = converterTipo(atacante[index_A].tipo) - converterTipo(defensor[index_B].tipo); 
    float ataque_atacante = atacante[index_A].ataque;
    float defesa_defensor = defensor[index_B].defesa;
    if (power_relation == -1 || power_relation == 4)
    {
        ataque_atacante *= Bonus_ataque;
    }
    else if (power_relation == 1 || power_relation == -4)
    {
        ataque_atacante *= Debuff_ataque;
        
    }

    if (ataque_atacante > defesa_defensor)
    {
        defensor[index_B].vida -= (ataque_atacante - defesa_defensor);

    }
    else
    {
        defensor[index_B].vida--;

    }
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
                return 2;
            }
        } 
        else 
        {
            if (sscanf(linha_buffer, "%s %d %d %f %s", treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].nome, &treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].ataque, &treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].defesa, &treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].vida, treinadorB->cartel_pokemon[i - treinadorA->num_pokemons].tipo) != 5)
            {
                printf("%s", linha_buffer);
                return 2;
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

int main() 
{
    treinador treinadorA, treinadorB;
    FILE *arq; 
    arq = fopen("text.txt", "r");     //abre o arquivo
    if (arq == NULL) {
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
        printf("Erro! Os pokemons precisam ter 5 caracteristicas (Nome, ataque, defesa, vida e tipo)\n");
        return 3;
    default:
        break;
    }

    int pok_A_index = 0, pok_B_index = 0;
    //BATALHAAAA!
    while (treinadorA.num_pokemons_vivos > 0 && treinadorB.num_pokemons_vivos > 0)
    {
        // A ataca B
        pokebatalha(treinadorA.cartel_pokemon, pok_A_index, treinadorB.cartel_pokemon, pok_B_index); //Pokemon do treinador A ataca o do B
        if (treinadorB.cartel_pokemon[pok_B_index].vida <= 0)   //verifica se o pokemon do A venceu.
        {
            printf("%s venceu %s\n", treinadorA.cartel_pokemon[pok_A_index].nome,treinadorB.cartel_pokemon[pok_B_index].nome);
            treinadorB.num_pokemons_vivos--;
            pok_B_index++;
        }
        //Verifica se o numero de pokemons de B chegou a zero para encerrar a batalha
        if (treinadorB.num_pokemons_vivos <= 0)
        {
            break;
        }
        // B ataca A.
        pokebatalha(treinadorB.cartel_pokemon, pok_B_index, treinadorA.cartel_pokemon, pok_A_index);
        if (treinadorA.cartel_pokemon[pok_A_index].vida <= 0) //verifica se o pokemon do B venceu.
        {
            printf("%s venceu %s\n", treinadorB.cartel_pokemon[pok_B_index].nome,treinadorA.cartel_pokemon[pok_A_index].nome);
            treinadorA.num_pokemons_vivos--;
            pok_A_index++;
        }
    }
    printf(treinadorA.num_pokemons_vivos == 0 ? "Jogador 2 venceu\n": "Jogador 1 venceu\n");
    mostrar_sobreviventes(&treinadorA, &treinadorB);
    mostrar_derrotados(&treinadorA, &treinadorB);
    printf("\n");
    return 0;
}
