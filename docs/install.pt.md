# Instalação

## Manual

### Instalando dependências:

- **Debian/Ubuntu**:

```bash
sudo apt install gcc make build-essential git libreadline-dev libncurses5-dev libncursesw5-dev
```

- **Arch Linux**:

```bash
sudo pacman -S gcc make base-devel git readline ncurses
```

### Compilação:

```bash
git clone https://github.com/JN513/Risco-5S.git
cd Risco-5S
make clean
make
```

### Adicionando ao PATH:

```bash
sudo cp simulador /usr/bin/risco_5s
```

### Desinstalando:

```bash
sudo rm /usr/bin/risco_5s
```
