﻿#include <iostream>

using namespace std;

// Cấu trúc của một node trong cây, đại diện cho các chương, mục, hoặc tiểu mục
struct TreeNode {
    char title[100]; // Tiêu đề của node
    int pageCount; // Số trang của node
    TreeNode** subNodes; // Mảng con trỏ đến các node con
    int subNodeCount; // Số lượng node con

    TreeNode(char* t, int p) : pageCount(p), subNodeCount(0), subNodes(nullptr) {
        for (int i = 0; i < 100; ++i) {
            title[i] = t[i];
            if (t[i] == '\0') break;
        }
    }
};

void addSubNode(TreeNode* parent, TreeNode* child) {
    TreeNode** newSubNodes = new TreeNode * [parent->subNodeCount + 1];
    for (int i = 0; i < parent->subNodeCount; ++i) {
        newSubNodes[i] = parent->subNodes[i];
    }
    newSubNodes[parent->subNodeCount] = child;
    delete[] parent->subNodes;
    parent->subNodes = newSubNodes;
    ++parent->subNodeCount;
}

void printTree(TreeNode* node, int depth = 0) {
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << "- ";
    for (int i = 0; i < 100; ++i) {
        if (node->title[i] == '\0') break;
        cout << node->title[i];
    }
    cout << " (" << node->pageCount << " trang)" << endl;
    for (int i = 0; i < node->subNodeCount; ++i) {
        printTree(node->subNodes[i], depth + 1);
    }
}

void deleteTreeNode(TreeNode* node) {
    for (int i = 0; i < node->subNodeCount; ++i) {
        deleteTreeNode(node->subNodes[i]);
    }
    delete[] node->subNodes;
    delete node;
}

int countChapters(TreeNode* root) {
    if (!root) return 0;
    return root->subNodeCount;
}

TreeNode* findLongestChapter(TreeNode* root) {
    if (!root) return nullptr;
    TreeNode* longestChapter = nullptr;
    int maxPages = 0;
    for (int i = 0; i < root->subNodeCount; ++i) {
        if (root->subNodes[i]->pageCount > maxPages) {
            maxPages = root->subNodes[i]->pageCount;
            longestChapter = root->subNodes[i];
        }
    }
    return longestChapter;
}

bool deleteByTitle(TreeNode* parent, const char* targetTitle) {
    for (int i = 0; i < parent->subNodeCount; ++i) {
        TreeNode* child = parent->subNodes[i];
        bool match = true;
        for (int j = 0; j < 100; ++j) {
            if (child->title[j] != targetTitle[j]) {
                match = false;
                break;
            }
            if (targetTitle[j] == '\0') break;
        }
        if (match) {
            deleteTreeNode(child);
            for (int j = i; j < parent->subNodeCount - 1; ++j) {
                parent->subNodes[j] = parent->subNodes[j + 1];
            }
            --parent->subNodeCount;
            return true;
        }
        else if (deleteByTitle(child, targetTitle)) {
            return true;
        }
    }
    return false;
}

int updatePageCount(TreeNode* node) {
    if (!node) return 0;
    int totalPageCount = node->pageCount;
    for (int i = 0; i < node->subNodeCount; ++i) {
        totalPageCount += updatePageCount(node->subNodes[i]);
    }
    node->pageCount = totalPageCount;
    return totalPageCount;
}

int main() {
    char mainTitle[] = "Muc luc";
    char chapter1[] = "Chuong 1: Gioi thieu";
    char chapter2[] = "Chuong 2: Kien thuc nang cao";
    char chapter3[] = "Chuong 3: Thuc hanh";
    char section1[] = "Phan 1.1: Lich su";
    char section2[] = "Phan 1.2: Dinh nghia";
    char section3[] = "Phan 2.1: Khai niem";
    char section4[] = "Phan 2.2: Bai tap";

    TreeNode* root = new TreeNode(mainTitle, 0);
    TreeNode* ch1 = new TreeNode(chapter1, 10);
    TreeNode* ch2 = new TreeNode(chapter2, 20);
    TreeNode* ch3 = new TreeNode(chapter3, 30);
    addSubNode(root, ch1);
    addSubNode(root, ch2);
    addSubNode(root, ch3);
    addSubNode(ch1, new TreeNode(section1, 5));
    addSubNode(ch1, new TreeNode(section2, 5));
    addSubNode(ch2, new TreeNode(section3, 10));
    addSubNode(ch2, new TreeNode(section4, 10));

    cout << "Muc luc cuon sach: " << endl;
    printTree(root);

    cout << "\nSo chuong trong sach: " << countChapters(root) << endl;

    TreeNode* longestChapter = findLongestChapter(root);
    if (longestChapter) {
        cout << "\nChuong dai nhat: ";
        for (int i = 0; i < 100; ++i) {
            if (longestChapter->title[i] == '\0') break;
            cout << longestChapter->title[i];
        }
        cout << " (" << longestChapter->pageCount << " trang)" << endl;
    }

    cout << "\nXoa 'Phan 1.2: Dinh nghia'..." << endl;
    if (deleteByTitle(root, "Phan 1.2: Dinh nghia")) {
        cout << "Xoa thanh cong!" << endl;
    }
    else {
        cout << "Khong tim thay muc can xoa!" << endl;
    }

    updatePageCount(root);

    cout << "\nMuc luc sau khi xoa muc: " << endl;
    printTree(root);

    deleteTreeNode(root);

    return 0;
}
