# myRPC-client (systemd)

## Что я устанавливаю?
`myRPC-client` - это утилита для взаимодействия с myRPC-server (см. [requiremenets.md](../requiremenets.md)).
- Формат запроса на выполнение команды:
```json
{“login”:”имя_пользователя”,”command”:”команда bash”}
```

Все логи программы записываются в заранее установленный файл логов с помощью libmysyslog.

## Установка
```sh
make deb
dpkg -i myRPC-client.deb
```
Этот набор команд установит `myRPC-client` в `/usr/bin/`.

## Использование

```sh
myRPC-client -c "echo hello!" -h 192.168.1.2 -p 7777 -s
```

Ознакомиться со списком доступных опций для клиента можно передав соответственно аргумент `--help` исполняемому файлу.

`--help` - напечатает полный список опций     
`--host / -h IP_ADDR` - IP хоста  
`--port / -p PORT` - порт хоста  
`--stream / -s` - использовать потоковой сокет  
`--dgram / -d` - использовать датаграмный сокет   
`--command / -c BASH_COMMAND` - команда для myRPC-server
