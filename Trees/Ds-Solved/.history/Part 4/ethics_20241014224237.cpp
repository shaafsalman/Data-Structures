// This function calculates the bias in the tree
template <class T, class S, class C>
vector<float> AVL<T, S, C>::bias(shared_ptr<node<T, S, C>> root) {
    vector<float> ratios;
    queue<shared_ptr<node<T, S, C>>> q;

    if (root) q.push(root);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        int maleCount = 0;
        int femaleCount = 0;

        // Count males and females in the left subtree
        function<void(shared_ptr<node<T, S, C>>)> countHelper = [&](shared_ptr<node<T, S, C>> node) {
            if (node) {
                if (node->gender == "Male") {
                    maleCount++;
                } else if (node->gender == "Female") {
                    femaleCount++;
                }
                countHelper(node->left);
                countHelper(node->right);
            }
        };

        countHelper(current->left); // Count in left subtree

        // Compute ratio only if maleCount is not zero to avoid division by zero
        float ratio = (maleCount > 0) ? static_cast<float>(femaleCount) / maleCount : 0;
        ratios.push_back(ratio);

        // Add children to the queue for next level
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return ratios;
}
