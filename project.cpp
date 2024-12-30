#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
// This is DOM Node and it is representing the Node contains elements of div such as tag and content
class DOMNode {
public:
    string tagname;
    string content;
    DOMNode* parent;
    DOMNode* firstchild;
    DOMNode* nextsibling;

    DOMNode( const string& tag, const string& text = "")
        : tagname(tag),content(text),parent(nullptr),firstchild(nullptr),
        nextsibling(nullptr) {}
};

class DOMTree {
    public:
    DOMNode* root;

    void render(DOMNode* node, ostream &outStream, int spacing) {
        if (node == nullptr){
            return;
        }
        outStream << string(spacing * 2, ' ') << "<" << node->tagname << ">" << endl;
        if (node->content != "")
        {
            outStream << string((spacing + 1)* 2, ' ') << node->content << endl;
        }
        render(node->firstchild,outStream, spacing+ 1);

        outStream << string(spacing * 2, ' ') << "</" << node->tagname << ">" << endl;
        render(node->nextsibling,outStream,spacing);
    }

    void delete_subtree(DOMNode* node){
        if (node == nullptr)
        {
            return;
        }
        delete_subtree(node->firstchild);
        delete_subtree(node->nextsibling);
        delete node;
    }


    DOMTree(const string& rootTag)
    {
        root = new DOMNode(rootTag);
    }

    ~DOMTree() {
        delete_subtree(root);
    }

    void addchild(DOMNode* parent, DOMNode* newNode) {
        if (parent == nullptr || newNode == nullptr) {
            return;
        }

        newNode->parent = parent;

        if (parent->firstchild == nullptr) {
            parent->firstchild = newNode;
        } else {
            DOMNode* sibling = parent->firstchild;
            while (sibling->nextsibling != nullptr) {
                sibling = sibling->nextsibling;
            }
            sibling->nextsibling = newNode;
        }
    }

    DOMNode* create_node(const string& tag, const string& content = "") {
        return new DOMNode(tag, content);
    }

    DOMNode* getRoot() {
        return root;
    }

    void savefile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            cout << "Error: Unable to open file for writing." << endl;
            return;
        }
        render(root, outFile, 0);
        outFile.close();
        cout << "DOM saved to file: " << filename << endl;
    }

    void display_DOM() {
        render(root, cout, 0);
    }

    void remove_node(DOMNode* node) {
        if (node == nullptr || node == root) {
            cout << "Error  Cannot remove the root node or a null node." << endl;
            return;
        }

        DOMNode* parent = node->parent;
        if (parent->firstchild == node) {
            parent->firstchild = node->nextsibling;
        } else {
            DOMNode* sibling = parent->firstchild;
            while (sibling->nextsibling != node) {
                sibling = sibling->nextsibling;
            }
            sibling->nextsibling = node->nextsibling;
        }

        delete_subtree(node);
        cout << "Node removed successfully." << endl;
    }

    DOMNode* find_node(DOMNode* node, const string& tag) {
        if (node == nullptr) return nullptr;
        if (node->tagname == tag) return node;

        DOMNode* foundInChild = find_node(node->firstchild, tag);
        if (foundInChild != nullptr) return foundInChild;

        return find_node(node->nextsibling, tag);
    }

    void view_node_content(const string& tag) {
        DOMNode* node = find_node(root, tag);
        if (node != nullptr) {
            cout << "Content of <" << node->tagname << ">: " << node->content << endl;
        } else {
            cout << "Error Tag not found" << endl;
        }
    }

    void editing_node_content(const string& tag, const string& newContent) {
        DOMNode* node = find_node(root, tag);
        if (node != nullptr) {
            node->content = newContent;
            cout << "Content updated successfully" << endl;
        } else {
            cout << "Error Tag not found" << endl;
        }
    }

    int count_nodes(DOMNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return 1 + count_nodes(node->firstchild) + count_nodes(node->nextsibling);
    }

    void move_node(DOMNode* nodeToMove, DOMNode* newParent){
        if (nodeToMove == nullptr || newParent == nullptr)
        {
            cout << "Error Node or new parent not found" << endl;
            return;
        }

        remove_node(nodeToMove);
        addchild(newParent, nodeToMove);
        cout << "Node moved successfully" << endl;
    }
};

void displayMenu() {
    cout << "\n--- DOM Visualizer Menu --" << endl;
    cout << "1. Add Element" << endl;
    cout << "2. Remove Element" << endl;
    cout << "3. View Element Content" << endl;
    cout << "4. Edit Element Content" << endl;
    cout << "5. Move Element" << endl;
    cout << "6. Display DOM" << endl;
    cout << "7. Save DOM to File" << endl;
    cout << "8. Count Total Elements" << endl;
    cout << "9. Exit" << endl;
    cout << "---------------------------" << endl;
}

int error_handling() {
    string input;
    int choice;
    while (true) {
        try {
            cout << "Enter your choice ";
            cin >> input;

            for (char c : input) {
                if (!isdigit(c)) {
                    throw invalid_argument("Invalid input Please enter a valid number");
                }
            }

            choice = stoi(input);
            break;
        } catch (const invalid_argument &e) {
            cout << e.what() << endl;
        } catch (...) {
            cout << "An unknown error occurred Please try again" << endl;
        }
    }
    return choice;
}

int main() {
    DOMTree dom("html");
    int choice;
    string tag, content, parentTag;

    while (true) {
        displayMenu();
        choice = error_handling();

        switch (choice) {
            case 1:
                cout << "Enter parent tag ";
                cin >> parentTag;
                cout << "Enter tag to add ";
                cin >> tag;
                cin.ignore();
                cout << "Enter content (not necessary): ";
                getline(cin, content);
                dom.addchild(dom.find_node(dom.getRoot(), parentTag), dom.create_node(tag, content));
                break;

            case 2:
                cout << "Enter tag to remove ";
                cin >> tag;
                dom.remove_node(dom.find_node(dom.getRoot(), tag));
                break;

            case 3:
                cout << "Enter tag to view content ";
                cin >> tag;
                dom.view_node_content(tag);
                break;

            case 4:
                cout << "Enter tag to edit";
                cin >> tag;
                cin.ignore();
                cout << "Enter new content ";
                getline(cin, content);
                dom.editing_node_content(tag, content);
                break;

            case 5:
                cout << "Enter tag to move ";
                cin >> tag;
                cout << "Enter new parent tag ";
                cin >> parentTag;
                dom.move_node(dom.find_node(dom.getRoot(), tag), dom.find_node(dom.getRoot(), parentTag));
                break;

            case 6:
                dom.display_DOM();
                break;

             case 7:  
                dom.savefile("dom_structure.html");
                break;

            case 8:  
                cout << "Total elements: " << dom.count_nodes(dom.getRoot()) << endl;
                break;

            case 9:  
                return 0;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
