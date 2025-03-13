# Comparação entre Implementações do Algoritmo de Dijkstra

## Introdução
Este relatório apresenta uma análise comparativa do desempenho do algoritmo de **Dijkstra** utilizando duas estruturas de dados diferentes para a fila de prioridades:

- **Heap Binária** (Fila de Prioridade Comum)
- **Heap de Fibonacci**

A avaliação considera o tempo de execução e o uso de memória para diferentes tamanhos de grafos.

---

## Metodologia
Foram testadas diferentes instâncias do problema, categorizadas em:

- **mp1, mp2** → Instâncias muito pequenas
- **p1, p2** → Instâncias médias
- **p3, p4** → Instâncias grandes
- **m1, m2, m3** → Instâncias muito grandes

Cada implementação foi executada e medidas de tempo e memória foram coletadas.

---

## Resultados

### **Tempo de Execução**

| Instância | Heap Binária (s) | Heap de Fibonacci (s) |
|-----------|-----------------|----------------------|
| mp1       | 0.000132        | 0.000052            |
| mp2       | 0.000112        | 0.000064            |
| p1        | 0.002108        | 0.002127            |
| p2        | 0.002393        | 0.002238            |
| p3        | 0.191006        | 0.221300            |
| p4        | 0.190916        | 0.217498            |
| m1        | 5.197069        | 5.354844            |
| m2        | 5.216550        | 5.371506            |
| m3        | 5.184763        | 5.367246            |

**Observações:**
- Para **instâncias pequenas e médias**, a Heap de Fibonacci foi **ligeiramente mais rápida**.
- Para **instâncias grandes**, a Heap Binária teve **melhor desempenho**.
- Para **instâncias muito grandes**, ambas tiveram **tempos semelhantes**, com uma leve vantagem para a Heap Binária.

### **Uso de Memória**

| Instância | Heap Binária (bytes) | Heap de Fibonacci (bytes) |
|-----------|---------------------|--------------------------|
| mp1       | 1,968               | 7,064                    |
| mp2       | 1,968               | 8,688                    |
| p1        | 321,512             | 342,304                  |
| p2        | 323,688             | 344,608                  |
| p3        | 30,576,648          | 30,871,072               |
| p4        | 30,589,384          | 30,882,776               |
| m1        | 762,220,296         | 763,777,624              |
| m2        | 762,262,344         | 763,828,072              |
| m3        | 762,253,704         | 763,826,224              |

**Observações:**
- A Heap de Fibonacci sempre utilizou **mais memória** do que a Heap Binária.
- Para instâncias pequenas, o overhead da Heap de Fibonacci foi **significativo**.
- Para instâncias médias e grandes, o uso de memória da Heap de Fibonacci foi **levemente maior**.
- Para instâncias muito grandes, a diferença de consumo de memória foi da ordem de **1 MB a mais** para a Heap de Fibonacci.

---

## Conclusão
1. **Heap de Fibonacci não trouxe um ganho expressivo no tempo de execução**, mesmo para instâncias grandes.
2. **Heap de Fibonacci usou mais memória em todos os casos**, devido à sua estrutura mais complexa.
3. **Para instâncias pequenas e médias, as diferenças de desempenho foram mínimas**, tornando a Heap Binária mais atrativa devido à sua simplicidade.
4. **Para instâncias muito grandes, a Heap Binária teve leve vantagem em tempo e consumo de memória**, tornando-a a melhor escolha na maioria dos cenários práticos.

### **Sugestões para Trabalhos Futuros**
- Testar os algoritmos em **grafos mais densos**, onde a operação `decrease-key` ocorre com mais frequência, para verificar se a Heap de Fibonacci teria alguma vantagem significativa.
- Avaliar o impacto da implementação de **Heap de Fibonacci otimizada** com uma estrutura melhor ajustada para cache e operações de hardware modernas.

---

## Referências
- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
- Dijkstra, E. W. (1959). "A Note on Two Problems in Connexion with Graphs." Numerische Mathematik, 1(1), 269-271.
