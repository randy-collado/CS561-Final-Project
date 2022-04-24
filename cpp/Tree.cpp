#include "Tree.hpp"

void Tree::init_serializer(std::string filename) {
  TS.openFile(filename, MODE::WRITE);
  ts_init = true;
}

S_Node *Tree::node_to_snode(
    Node *node) { // returns a pointer declared by new, user should delete
  S_Node *s_node = new S_Node();
  s_node->id = node->key;

  std::memcpy(s_node->payloads, node->values, node->numValues);

  for (size_t i = 0; i < node->numChildren; ++i) {
    s_node->children[i] = node->children[i]->key;
  }
  return s_node;
}

void Tree::dump_tree() { dump_tree_impl(this->head); }

void Tree::add(int key, int value) { add_impl(key, value); }

void Tree::fill(long numInserts, long upperBound, std::vector<int> &keys,
                std::vector<int> &values) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, upperBound);
  for (long i = 0; i < numInserts; ++i) {
    int key = distrib(gen);
    int value = distrib(gen);
    keys.push_back(key);
    values.push_back(value);
    add(key, value);
  }
}

void Tree::add_impl(int key, int value) {
  std::deque<Node *> queue = std::deque<Node *>();
  if (head == nullptr) {
    head = new Node(branch);
    head->key = key;
    head->values[head->numValues++] = value;
    head->level = 0;
    return;
  }
  queue.push_back(head);
  while (!queue.empty()) {
    Node *currentNode = queue.front();
    queue.pop_front();
    if (currentNode == nullptr) {
      assert(false && "unreachable");
    } else if (currentNode->key == key) {
      if (currentNode->numValues < currentNode->maxValues)
        currentNode->values[currentNode->numValues++] = value;
      return;
    } else if (currentNode->numChildren < branch) {
      Node *node = new Node(branch);
      node->key = key;
      node->values[node->numValues++] = value;
      node->level = currentNode->level + 1;
      if (node->level > maxLevel){
        maxLevel = node->level;
      }
      currentNode->children[currentNode->numChildren++] = node;
      return;
    } else {
      queue.insert(queue.end(), currentNode->children,
                   currentNode->children + currentNode->numChildren);
    }
  }
}

void Tree::dump_tree_impl(Node *head) {
  if (!ts_init) {
    std::cerr << "[ERROR] Serializer was not initialized" << std::endl;
    exit(1);
  }
  if (!head)
    return;
  auto s_node_ptr = node_to_snode(head);
  TS.writeNode(s_node_ptr);
  delete s_node_ptr;
  for (size_t i = 0; i < head->numChildren; ++i) {
    dump_tree_impl(head->children[i]);
  }
}

void Tree::dump_tree_tiered_impl(Node *head) {
  if (!ts_init) {
    std::cerr << "[ERROR] Serializer was not initialized" << std::endl;
    exit(1);
  }
  if (!head)
    return;

  tier_offsets *tiers = new tier_offsets();
  tiers->tiers++;
  tiers->offsets[0] = sizeof(tier_offsets);

  std::deque<Node *> queue = std::deque<Node *>();
  queue.push_back(head);
  size_t currentLevel = head->level;
  while (!queue.empty()) {
    Node *currentNode = queue.front();
    queue.pop_front();
    if (currentNode == nullptr)
      assert(false && "Unreachable");
    if (currentLevel != currentNode->level) {
      tiers->offsets[tiers->tiers++] = TS.get_current_offset();
      currentLevel = currentNode->level;
    }
    auto s_node_ptr = node_to_snode(head);
    TS.writeNode(s_node_ptr);
    delete s_node_ptr;
    queue.insert(queue.end(), currentNode->children,
                 currentNode->children + currentNode->numChildren);
  }
  TS.write_offset_metadata(tiers);
}

S_Node *Tree::node_to_aligned_snode(Node *node) {
#ifdef _WIN32
  S_Node *s_node = (S_Node *)_aligned_malloc(512, sizeof(S_Node));
#else
  S_Node *s_node = (S_Node *)aligned_alloc(512, sizeof(S_Node));
#endif

  s_node->id = node->key;

  std::memcpy(s_node->payloads, node->values, node->numValues);

  for (size_t i = 0; i < node->numChildren; ++i) {
    s_node->children[i] = node->children[i]->key;
  }
  return s_node;
}
