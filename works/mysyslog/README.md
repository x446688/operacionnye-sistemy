# libmysyslog

Согласно ТЗ: расширяемая библиотека libmysyslog на языке Си, которая может выводить данные в журнал в разных форматах. В рамках библиотеки реализована функция, через которую другие программы могут выводить данные в журнал.  

Для сборки модуля libmysyslog-json необходим установленнй libjson-c.

# Минимальный тест

## MacOS

```bash
# NOTE: BUILDX HAS TO BE INSTALLED!!!!
docker build --platform=linux/amd64 -t dolphian .  
docker run -it --rm --platform linux/amd64 dolphian /bin/bash
```

*Далее нужно следовать указаниям из инструкции для Linux*

## Linux

```bash
make
make run
```

## Universal (MacOS, GNU/Linux)

```bash
chmod +x bootstrap.sh
./bootstrap.sh
```

## Тестирование компонент (без установки)

```bash
LD_LIBRARY_PATH=libmysyslog:libmysyslog-text:libmysyslog-json ./libmysyslog-client/syslog-client --msg "success" --path "log.txt" --driver "2" -f "1" -l "1"
```