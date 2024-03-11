#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Contact {
public:
    std::string name;
    std::string phone;
    std::string email;

public:
    Contact(const std::string& name, const std::string& phone, const std::string& email)
            : name(name), phone(phone), email(email) {}

    friend std::ostream& operator<<(std::ostream& os, const Contact& contact) {
        os << contact.name << " | " << contact.phone << " | " << contact.email;
        return os;
    }
};

class ContactManager {
private:
    std::vector<Contact> contacts;

public:
    void readContacts() {
        std::ifstream file("phone.txt");
        if (!file) {
            return;
        }
        std::string line;
        while (std::getline(file, line) && contacts.size() < 100) {
            size_t comma1 = line.find(',');
            size_t comma2 = line.find(',', comma1 + 1);
            std::string name = line.substr(0, comma1);
            std::string phone = line.substr(comma1 + 1, comma2 - comma1 - 1);
            std::string email = line.substr(comma2 + 1);
            contacts.emplace_back(name, phone, email);
        }
        file.close();
        sortContacts();
    }

    void sortContacts() {
        std::sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
            return a.name < b.name;
        });
    }

    void saveContacts() {
        std::ofstream file("phone.txt");
        if (!file) {
            std::cout << "Error saving contacts." << std::endl;
            return;
        }
        for (const Contact& contact : contacts) {
            file << contact.name << "," << contact.phone << "," << contact.email << std::endl;
        }
        file.close();
    }

    void printContacts() const {
        std::cout << "Contacts:" << std::endl;
        for (size_t i = 0; i < contacts.size(); i++) {
            std::cout << i + 1 << ". " << contacts[i] << std::endl;
        }
    }

    void addContact() {
        if (contacts.size() == 100) {
            std::cout << "Max contacts reached." << std::endl;
            return;
        }
        std::string name, phone, email;
        std::cout << "Enter contact name: ";
        std::getline(std::cin >> std::ws, name);
        std::cout << "Enter phone number: ";
        std::getline(std::cin >> std::ws, phone);
        std::cout << "Enter email address: ";
        std::getline(std::cin >> std::ws, email);
        contacts.emplace_back(name, phone, email);
        std::cout << "Contact added." << std::endl;
    }

    void deleteContact() {
        std::cout << "Enter the name or number of the contact to delete: ";
        std::string input;
        std::getline(std::cin >> std::ws, input);

        auto it = std::find_if(contacts.begin(), contacts.end(), [&](const Contact& contact) {
            return contact.name == input;
        });

        if (it == contacts.end()) {
            std::cout << "Contact not found." << std::endl;
            return;
        }

        if (std::count_if(contacts.begin(), contacts.end(), [&](const Contact& contact) {
            return contact.name == input;
        }) > 1) {
            std::cout << "Multiple contacts with the same name found:" << std::endl;
            for (size_t i = 0; i < contacts.size(); i++) {
                if (contacts[i].name == input) {
                    std::cout << i + 1 << ": " << contacts[i].name << std::endl;
                }
            }
            std::cout << "Enter the number of the contact to delete: ";
            int selection;
            std::cin >> selection;
            if (selection >= 1 && selection <= contacts.size()) {
                contacts.erase(contacts.begin() + selection - 1);
                std::cout << "Contact deleted." << std::endl;
            } else {
                std::cout << "Invalid selection." << std::endl;
            }
        } else {
            contacts.erase(it);
            std::cout << "Contact deleted." << std::endl;
        }
    }

    void searchContact() const {
        std::cout << "Enter contact name to search: ";
        std::string name;
        std::getline(std::cin >> std::ws, name);

        auto it = std::find_if(contacts.begin(), contacts.end(), [&](const Contact& contact) {
            return contact.name == name;
        });

        if (it == contacts.end()) {
            std::cout << "Contact not found." << std::endl;
        } else {
            std::cout << "Contact found." << std::endl;
            std::cout << *it << std::endl;
        }
    }

    void updateContact() {
        std::cout << "Enter the name of the contact to update: ";
        std::string name;
        std::getline(std::cin >> std::ws, name);

        auto it = std::find_if(contacts.begin(), contacts.end(), [&](const Contact& contact) {
            return contact.name == name;
        });

        if (it == contacts.end()) {
            std::cout << "Contact not found." << std::endl;
            return;
        }

        std::cout << "Enter the new phone number: ";
        std::getline(std::cin >> std::ws, it->phone);
        std::cout << "Enter the new email address: ";
        std::getline(std::cin >> std::ws, it->email);
        std::cout << "Contact updated." << std::endl;
    }
};

int main() {
    ContactManager contactManager;
    contactManager.readContacts();
    int choice = 0;
    while (choice != 6) {
        std::cout << "\nPhone Contact Manager" << std::endl;
        std::cout << "1. View contacts" << std::endl;
        std::cout << "2. Add contact" << std::endl;
        std::cout << "3. Update contact" << std::endl;
        std::cout << "4. Delete contact" << std::endl;
        std::cout << "5. Search contact" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                contactManager.printContacts();
                break;
            case 2:
                contactManager.addContact();
                contactManager.saveContacts();
                break;
            case 3:
                contactManager.updateContact();
                contactManager.saveContacts();
                break;
            case 4:
                contactManager.deleteContact();
                contactManager.saveContacts();
                break;
            case 5:
                contactManager.searchContact();
                break;
            case 6:
                std::cout << "Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    return 0;
}
