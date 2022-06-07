# cg2022


## Configuração do ambiente

A parte prática da disciplina será programada na linguagem C e assume que:
- o sistema operacional utilizado será Linux x64. Sugestão: `Linux Mint 20.3`
- gcc e dependências estão instaladas.
  - No `Linux Mint` elas podem ser instaladas com o comando: `apt install build-essential libx11-dev -y`

Embora não seja obrigatório, sugere-se a utilização da IDE Visual Studio Code (VS Code), que requer poucas configurações e oferece conveniências como syntax highlighting, code formatting, entre outras.

### Usando máquina virtual

Para quem utiliza máquinas virtuais, neste [link](borken) há uma Oracle VirtuaBox VM pré-configurada.
- usuário `user`
- senha `password`

Para quem não pode ou não quer utilizar uma máquina virtual, continue lendo :) não é complicado configurar manualmente.

### Configuração manual

Para utilizar o VS Code:
- Download o .tar.gz clicando [(aqui)](https://code.visualstudio.com/download#)
- Extraia o .tar.gz, isso vai criar um diretório chamado `VSCode-linux-x64`
- Navegue até a pasta `VSCode-linux-x64/bin` e execute o programa `code`
  - caso você seja prompted sobre `Run in Terminal` ou `Run`, escolha `Run`: ![run](./figures/run_terminal.png)
- Com o VS Code aberto, pressione `ctrl+shift+x` para abrir o menu de extensões, procure e instale a extensão `C/C++ Extension Pack` ![extension](./figures/extension.png)

Se deu tudo certo, o VS Code deve estar configurado para reconhecer código em C, mas ainda precisamos indicar qual versão de C utlizamos:
- pressione `ctrl+,` para abrir o menu `Settings`
- utilize a função de busca para encontrar a opção `c_cpp standard` e selecione c17 ![cstd](./figures/cstd.png)

A última etapa, opcional, é habilitar a opção "formatar código toda vez que um arquivo for salvo": no mesmo menu `Settings` da etapa anterior, busque a opção `Format On Save` e marque a caixinha ![format](./figures/format_on_save.png)

### Verificando

Para verificar se tudo está configurado corretamente:
- Clone o repositório `https://github.com/Mirandatz/cg2022`
- Abra a pasta do repositório no VS Code
- Abra o `terminal integrado` do VS Code pressionando ``ctrl+shift+` `` e execute o comando `make`, se uma janela com uma imagem rosa abrir, então está tudo configurado :)
