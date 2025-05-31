# Запуск в докере:
docker build -t lexparser .
# запуск с передачей файла
docker run --rm -v ${PWD}/CodeExamples:/data lexparser /data/Example.cs
