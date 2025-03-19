import random
import time
import threading
import socket
import select
import struct
import os
import sys

class ServerCoLocation:
    def __init__(self, server_ip, server_port):
        self.server_ip = server_ip
        self.server_port = server_port
        self.client_sockets = []
        self.server_socket = None
        self.is_running = False

    def start_server(self):
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.bind((self.server_ip, self.server_port))
        self.server_socket.listen(5)
        self.is_running = True
        print(f"Server started at {self.server_ip}:{self.server_port}")
        self.accept_connections()

    def accept_connections(self):
        while self.is_running:
            try:
                client_socket, client_address = self.server_socket.accept()
                print(f"New connection from {client_address}")
                self.client_sockets.append(client_socket)
                threading.Thread(target=self.handle_client, args=(client_socket,)).start()
            except Exception as e:
                print(f"Error accepting connections: {e}")
                break

    def handle_client(self, client_socket):
        while self.is_running:
            try:
                data = client_socket.recv(1024)
                if not data:
                    break
                self.process_data(client_socket, data)
            except Exception as e:
                print(f"Error handling client data: {e}")
                break
        client_socket.close()

    def process_data(self, client_socket, data):
        print(f"Received data: {data}")
        response = self.generate_response(data)
        self.send_response(client_socket, response)

    def generate_response(self, data):
        return f"Processed data: {data}"

    def send_response(self, client_socket, response):
        try:
            client_socket.send(response.encode())
        except Exception as e:
            print(f"Error sending response: {e}")

    def stop_server(self):
        self.is_running = False
        for client_socket in self.client_sockets:
            client_socket.close()
        self.server_socket.close()
        print("Server stopped.")

class ClientSimulation:
    def __init__(self, server_ip, server_port, client_id):
        self.server_ip = server_ip
        self.server_port = server_port
        self.client_id = client_id

    def start_client(self):
        try:
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((self.server_ip, self.server_port))
            print(f"Client {self.client_id} connected to server")
            self.send_data(client_socket)
        except Exception as e:
            print(f"Error in client {self.client_id}: {e}")

    def send_data(self, client_socket):
        for i in range(10):
            data = f"Client {self.client_id} message {i}"
            client_socket.send(data.encode())
            time.sleep(random.uniform(0.5, 2.0))
            response = client_socket.recv(1024)
            print(f"Client {self.client_id} received: {response.decode()}")
        client_socket.close()

class CoLocationEnvironment:
    def __init__(self, server_ip, server_port, num_clients):
        self.server_ip = server_ip
        self.server_port = server_port
        self.num_clients = num_clients
        self.server = ServerCoLocation(server_ip, server_port)
        self.clients = []

    def start_environment(self):
        threading.Thread(target=self.server.start_server).start()
        for i in range(self.num_clients):
            client = ClientSimulation(self.server_ip, self.server_port, i + 1)
            self.clients.append(client)
            threading.Thread(target=client.start_client).start()

    def stop_environment(self):
        self.server.stop_server()
        print("All clients and server stopped.")

def monitor_resources():
    while True:
        cpu_usage = os.popen("top -bn1 | grep 'Cpu(s)'").readline()
        memory_usage = os.popen("free -m").readlines()[1]
        print(f"CPU Usage: {cpu_usage.strip()}")
        print(f"Memory Usage: {memory_usage.strip()}")
        time.sleep(5)

def run_server_and_clients():
    server_ip = "127.0.0.1"
    server_port = 9999
    num_clients = 5

    environment = CoLocationEnvironment(server_ip, server_port, num_clients)
    environment.start_environment()

    monitor_thread = threading.Thread(target=monitor_resources)
    monitor_thread.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        environment.stop_environment()
        monitor_thread.join()

if __name__ == "__main__":
    run_server_and_clients()
