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
