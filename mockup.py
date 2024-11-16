import socket
import threading
import time
from enum import Enum, auto

# Different scenarious 
class TestScenario(Enum):
    HAPPY_PATH = auto()
    CORRUPT_MESSAGE = auto()
    SERVER_DISCONNECT = auto()
    SLOW_RESPONSE = auto()
    AUTH_FAILURE = auto()
    MALFORMED_RESPONSE = auto()

class MockIMAPServer:
    def __init__(self, host='localhost', port=3143):
        self.host = host
        self.port = port
        self.running = False
        self.server_socket = None
        self.test_scenario = TestScenario.HAPPY_PATH

        # Mock email messages
        self.messages = {
            1: {
                'subject': 'Test 1',
                'from': 'sender1@example.com',
                'date': 'Tue, 12 Nov 2024 10:00:00 +0000',
                'body': 'This is test email content.\r\n',
            },
            2: {
                'subject': 'Test 2',
                'from': 'sender2@example.com',
                'date': 'Tue, 12 Nov 2024 11:00:00 +0000',
                'body': 'This is test email content.\r\n',
            },
        }

    def set_scenario(self, scenario: TestScenario):
        self.test_scenario = scenario
        print(f"Test scenario changed to: {scenario.name}")

    def format_message(self, msg_id, msg, full=True):
        headers = (
            f"From: {msg['from']}\r\n"
            f"Subject: {msg['subject']}\r\n"
            f"Date: {msg['date']}\r\n"
            f"Message-ID: <{msg_id}@mockserver.local>\r\n"
        )
        return f"{headers}\r\n{msg['body']}" if full else headers

    def send_response(self, client_socket, response):
        try:
            # Simulate scenarios
            if self.test_scenario == TestScenario.SLOW_RESPONSE:
                time.sleep(2)
            elif self.test_scenario == TestScenario.CORRUPT_MESSAGE:
                response = response[:-5] + "CORRUPTED\r\n"
            elif self.test_scenario == TestScenario.MALFORMED_RESPONSE:
                response = response.replace("OK ", "OK") + "MALFORMED"

            client_socket.send(response.encode('utf-8'))

            if self.test_scenario == TestScenario.SERVER_DISCONNECT:
                time.sleep(0.5)
                client_socket.close()
        except Exception as e:
            print(f"Error sending response: {e}")

    def handle_client(self, client_socket, client_address):
        """Handle communication with a connected client."""
        try:
            # Initial greeting
            self.send_response(client_socket, "* OK IMAP4rev1 Mock Server Ready\r\n")
            authenticated = False

            while True:
                try:
                    data = client_socket.recv(1024).decode('utf-8').strip()
                    if not data:
                        break
                    print(f"Received: {data}")
                except ConnectionError:
                    break

                # Parse the command
                parts = data.split()
                if len(parts) < 2:
                    continue

                tag, command = parts[0], parts[1].upper()

                # Command handling
                if command == "LOGIN":
                    if self.test_scenario == TestScenario.AUTH_FAILURE:
                        self.send_response(client_socket, f"{tag} NO [AUTHENTICATIONFAILED] Invalid credentials\r\n")
                    else:
                        authenticated = True
                        self.send_response(client_socket, f"{tag} OK LOGIN completed\r\n")
                elif command == "CAPABILITY":
                    capabilities = "* CAPABILITY IMAP4rev1 AUTH=PLAIN\r\n"
                    self.send_response(client_socket, capabilities)
                    self.send_response(client_socket, f"{tag} OK CAPABILITY completed\r\n")
                elif command == "SELECT":
                    self.send_response(client_socket, f"* {len(self.messages)} EXISTS\r\n* 0 RECENT\r\n{tag} OK [READ-WRITE] SELECT completed\r\n")
                elif command == "SEARCH":
                    self.send_response(client_socket, "* SEARCH 1 2\r\n")
                    self.send_response(client_socket, f"{tag} OK SEARCH completed\r\n")
                elif command == "FETCH":
                    try:
                        msg_id = int(parts[2])
                        if msg_id in self.messages:
                            msg = self.messages[msg_id]
                            full_fetch = "BODY[]" in data
                            content = self.format_message(msg_id, msg, full_fetch)
                            response = f"* {msg_id} FETCH (RFC822 {{{len(content)}}}\r\n{content})\r\n"
                            self.send_response(client_socket, response)
                            self.send_response(client_socket, f"{tag} OK FETCH completed\r\n")
                    except (ValueError, IndexError):
                        self.send_response(client_socket, f"{tag} BAD Invalid FETCH command\r\n")
                elif command == "LOGOUT":
                    self.send_response(client_socket, "* BYE IMAP4rev1 Server logging out\r\n")
                    self.send_response(client_socket, f"{tag} OK LOGOUT completed\r\n")
                    break
                else:
                    self.send_response(client_socket, f"{tag} BAD Unknown command\r\n")
        except Exception as e:
            print(f"Error handling client {client_address}: {e}")
        finally:
            client_socket.close()

    def start(self):
        """Start the mock IMAP server."""
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)
        self.running = True

        print(f"Mock IMAP server listening on {self.host}:{self.port}")
        print(f"Current test scenario: {self.test_scenario.name}")

        while self.running:
            try:
                client_socket, client_address = self.server_socket.accept()
                print(f"New connection from {client_address}")
                threading.Thread(target=self.handle_client, args=(client_socket, client_address)).start()
            except Exception as e:
                if self.running:
                    print(f"Error accepting connection: {e}")

    def stop(self):
        """Stop the mock IMAP server."""
        self.running = False
        if self.server_socket:
            self.server_socket.close()

if __name__ == "__main__":
    server = MockIMAPServer()
    try:
        server_thread = threading.Thread(target=server.start)
        server_thread.start()

        print("\nServer running. Press Ctrl+C to stop.")
        scenarios = list(TestScenario)
        for idx, scenario in enumerate(scenarios, 1):
            print(f"{idx}. {scenario.name}")

        while True:
            command = input("\nEnter scenario number or name (Ctrl+C to exit): ").strip()
            try:
                if command.isdigit():
                    index = int(command) - 1
                    if 0 <= index < len(scenarios):
                        server.set_scenario(scenarios[index])
                    else:
                        print(f"Invalid scenario number")
                else:
                    server.set_scenario(TestScenario[command.upper()])
            except (ValueError, KeyError):
                print("Invalid input")
    except KeyboardInterrupt:
        print("\nStopping server...")
        server.stop()
