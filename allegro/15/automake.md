Aqui está um script eficiente usando `inotifywait` para executar `make` automaticamente:Criei um script completo! Aqui está como usar:

## **Instalação**

```bash
# Instale inotify-tools
sudo apt-get install inotify-tools

# Torne o script executável
chmod +x automake.sh
```

## **Uso básico**

```bash
# Monitorar diretório atual
./automake.sh

# Monitorar diretório específico
./automake.sh /caminho/para/projeto

# Monitorar extensões específicas
./automake.sh . "c,cpp,h"
```

## **Recursos principais**

✅ **Debounce** - Evita builds múltiplos em rápida sucessão
✅ **Filtro de extensões** - Só recompila quando arquivos relevantes mudam
✅ **Exclusão inteligente** - Ignora `.git`, `.o`, `build/`, etc.
✅ **Output colorido** - Fácil de visualizar status
✅ **Build inicial** - Executa make ao iniciar
✅ **Notificações** - Alerta visual em caso de erro (se disponível)

## **Alternativa simples (one-liner)**

Se preferir algo mais direto:

```bash
while inotifywait -r -e modify,create .; do make; done
```

## **Integração com Neovim**

Para executar diretamente do Neovim, adicione ao seu config:

```lua
vim.keymap.set("n", "<leader>mw", function()
    vim.cmd("split | terminal ./automake.sh")
end, { desc = "Iniciar auto-make watch" })
```

O script monitora recursivamente todos os arquivos e executa `make` automaticamente quando detecta mudanças!
