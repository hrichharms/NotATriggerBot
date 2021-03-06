#include <windows.h>
#include <wtypes.h>
#include <fstream>
#include <string>
#include <stack>
#include <iostream>


// number of possible values per channel
const int BRANCH_FACTOR = 256;


void get_pixel_values(HDC device_handle, int x, int y, int* r, int* g, int* b) {
    COLORREF rgb = GetPixel(device_handle, x, y);
    *r = GetRValue(rgb);
    *g = GetGValue(rgb);
    *b = GetBValue(rgb);
}


struct Trie {
    struct Trie* child[BRANCH_FACTOR];
    int r = 0;
    int g = 0;
    int b = 0;
};


struct Trie* create_node() {
    struct Trie* node = new Trie;
    for (int i = 0; i < BRANCH_FACTOR; i++)
        node->child[i] = NULL;
    return node;
}


void insert(struct Trie* root, int r, int g, int b) {
    if (!root->child[r]) {
        root->child[r] = create_node();
    }
    if (!root->child[r]->child[g]) {
        root->child[r]->child[g] = create_node();
    }
    if (!root->child[r]->child[g]->child[b]) {
        root->child[r]->child[g]->child[b] = create_node();
    }
    root->child[r]->child[g]->child[b]->r = r;
    root->child[r]->child[g]->child[b]->g = g;
    root->child[r]->child[g]->child[b]->b = b;
}


bool search(struct Trie* root, int r, int g, int b) {
    if (!root->child[r]) {
        return false;
    }
    if (!root->child[r]->child[g]) {
        return false;
    }
    if (!root->child[r]->child[g]->child[b]) {
        return false;
    }
    return true;
}


struct Trie* load_trie(int* n, std::string filename) {
    std::cout << "Loading trie data from " << filename << "..." << std::endl;
    int r, g, b;
    std::ifstream in_file(filename);
    in_file >> *n;
    std::cout << "Loading " << *n << " color value(s) into trie datastructure..." << std::endl;
    struct Trie* root = create_node();
    for (int i = 0; i < *n; i++) {
        in_file >> r >> g >> b;
        std::cout << "\t" << r << " " << g << " " << b << std::endl;
        insert(root, r, g, b);
    }
    in_file.close();
    return root;
}


void save_trie(int n, struct Trie* root, std::string filename) {
    std::cout << "Saving " << n << " color values to " << filename << "..." << std::endl;
    std::ofstream out_file(filename);
    out_file << n;
    std::stack<struct Trie*> trie_stack;
    trie_stack.push(root);
    struct Trie* node;
    bool children = 0;
    while (!trie_stack.empty()) {
        node = trie_stack.top();
        trie_stack.pop();
        children = 0;
        for (int i = 0; i < BRANCH_FACTOR; i++) {
            if (node->child[i] != NULL) {
                trie_stack.push(node->child[i]);
                children = 1;
            }
        }
        if (!children) {
            std::cout << "\t" << node->r << " " << node->g << " " << node->b << std::endl;
            out_file << " " << node->r << " " << node->g << " " << node->b;
        }
    }
    out_file.close();
}


int main() {

    // load trie data
    int n;
    struct Trie* values_tree = load_trie(&n, "data.txt");

    // get window and device handles
    HWND desktop_window = GetDesktopWindow();
    HDC device_handle = GetDC(desktop_window);

    // get desktop screen dimensions
    RECT desktop;
    GetWindowRect(desktop_window, &desktop);
    int WIDTH = desktop.right;
    int HEIGHT = desktop.bottom;

    // calculate sample pixel coordinates
    int check_x = WIDTH / 2 + 2;
    int check_y = HEIGHT / 2 + 2;

    // macro settings
    int control_key = 0x54;
    int save_key = 0x52;

    // pixel value integers
    int r, g, b;

    // macro loop
    while (1) {

        while (GetAsyncKeyState(control_key)) {
            // get pixel values and insert them into values tree
            get_pixel_values(device_handle, check_x, check_y, &r, &g, &b);
            if (!search(values_tree, r, g, b)) {
                std::cout << "Found new color! Inserting " << r << " " << g << " " << b << " into trie datastructure..." << std::endl;
                insert(values_tree, r, g, b);
                n++;
            }
        }

        // check for user save input
        if (GetAsyncKeyState(save_key)) {
            // save trie data
            save_trie(n, values_tree, "data.txt");
            // wait for key to be released before continuing
            while (GetAsyncKeyState(save_key)) {}
        }

    }

    return 0;
}
