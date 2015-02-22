Findpat2
==============================

------------------------------
Autores
------------------------------

Alberto Trindade Tavares (att)
Bruna Carolina Baudel de Santana (bcbs)

------------------------------
Instruções de Compilação
------------------------------

Para gerar o executável, execute os comandos:
- cd src/
- make
O executável de findpat2 é gerado e disponibilizado no diretório /bin

Para apagar o diretório /bin com o executável, execute os comandos:
- cd src/
- make clean

Essas instruções consideram que o usuário está na pasta raiz do projeto. Se o usuário já estiver no diretório /src, não é necessário o primeiro comando.

------------------------------
Instruções de Uso
------------------------------

Uso: ./findpat2 MODE [OPTIONS] PARAM1 ... PARAMN
Este programa tem dois modos de execução: INDEXAÇÃO e BUSCA
MODE deve ser especificado como 'index' ou 'search', indicando um desses modos
Cada modo requer uma lista específica de parâmetros (PARAM1, ..., PARAMN) e 
opções (OPTIONS), como descrito a seguir

Uso no modo de indexação: ./findpat2 index [OPTIONS] TEXTFILE
Produz um arquivo de índice para TEXTFILE, permitindo futuras buscas de padrões
em TEXTFILE, através do modo de busca da ferramenta
O arquivo do índice é nomeado como TEXTFILE_BASENAME.idx. Por exemplo, "findpat2 index
textile.txt" produz um arquivo com o nome "textfile.idx"

Opções (OPTIONS):
  -h, --help     Imprime instruções de uso (em Inglês)

Uso no modo de busca: ./findpat2 search [OPTIONS] PATTERN INDEXFILE
Esse comando busca por ocorrências de PATTERN no arquivo de texto indexado em INDEXFILE, 
que deve ser um arquivo com extensão .idx gerado por este programa no modo de indexação
Se a opção --pattern for setada, uma lista de padrões será usada no lugar de PATTERN

Opções (OPTIONS):
  -h, --help     Imprime instruções de uso (em Inglês)
  -p, --pattern  Especifica um arquivo com padrões a serem buscados, um por linha,
                 para serem usados no lugar de PATTERN
  -c, --count    Ao invés de imprimir as linhas do texto indexado em INDEXFILE em que os 
                 padrões ocorrem, a quantidade total de ocorrências é exibida

------------------------------
Exemplos de Uso
------------------------------

Modo de indexação:
- Gera um arquivo de índice textfile1.idx para textfile1.txt: ./findpat2 index textfile1.txt

Modo de busca:
- Imprime linhas do texto indexado em textfile1.idx onde "ababc" ocorre: ./findpat2 search ababc textfile1.idx
- Imprime a quantidade de ocorrências de "ababc" no texto indexado em textfile2.idx: ./findpat2 search -c ababc textfile2.idx
- Imprime a quantidade de ocorrências de padrões listados em patternfile.txt, para o texto indexado em textfile1.idx: ./findpat2 search -c -p patternfile.txt textfile1.idx