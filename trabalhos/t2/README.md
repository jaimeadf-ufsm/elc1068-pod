# Projeto de Compressão e Busca de Arquivos


Este projeto implementa duas funcionalidades principais:

1. Busca Binária: Implementação da busca binária para localizar registros nos arquivos, ordenando-os pelo CNPJ e nome.

2. Compressão de Dados: Utilizamos a compressão LZ78 para compactar os arquivos.


## Instalação

Clone o repositório:

```bash
git clone https://github.com/lgtozevich-ufsm/elc1068-pod

cd trabalhos/t2
```

## Compilação
Para compilar o projeto, execute o seguinte comando:

```bash
make all

```

## Instruções de Uso

### Busca

Para executar o programa de busca:

```bash
./bin/search <dados.EMPRECSV>
```
Substitua <dados.EMPRECSV> pelo caminho para o arquivo CSV contendo os registros.

### Compressão e descompressão de dados

Para executar o programa de compressão:

```bash
./bin/wd41 C <dados.EMPRECSV> <dados_comprimidos.wd41>
```

Substitua <dados.EMPRECSV> pelo arquivo de entrada e <dados_comprimidos.wd41> pelo arquivo compactado de saída.

Para executar a descompressão:

```bash
./bin/wd41 D <dados_comprimidos.wd41> <dados_descomprimidos.EMPRECSV>
```

Substitua <dados_comprimidos.wd41> pelo arquivo compactado e <dados_descomprimidos.EMPRECSV> pelo arquivo descompactado de saída.

## Taxa de compressão obtida

No experimento realizado com o arquivo <Empresas0.EMPRECSV>, que inicialmente possuía 1.352.694 KB, aplicamos a compressão utilizando nosso algoritmo baseado no método LZ78. Após a compressão, o tamanho do arquivo foi reduzido para 349.846 KB, resultando em uma taxa de compressão de aproximadamente 25,26%.

### Comparação com o Formato .zip

Para avaliar o desempenho do nosso método, realizamos uma compressão comparativa utilizando o popular formato .zip. O mesmo arquivo <Empresas0.EMPRECSV> foi comprimido com .zip, resultando em um tamanho final de 341.734 KB. Embora o formato .zip tenha produzido um arquivo ligeiramente menor (cerca de 8.112 KB a menos), a diferença entre as duas compressões é pequena, representando apenas uma melhoria de cerca de 2,32% em relação ao nosso compressor LZ78.
