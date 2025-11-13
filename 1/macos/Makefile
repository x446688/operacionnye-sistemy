.DEFAULT_GOAL := Dockerfile
prep:
	colima start docker
Dockerfile: prep
	docker build -t lab .
run: 
	docker run -it --mount type=bind,src=../,dst=/home/user/ --rm lab 
