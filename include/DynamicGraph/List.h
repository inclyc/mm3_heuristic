#ifndef LIST_H
#define LIST_H
namespace DynamicGraph {
struct List {
  int *head;
  int *pre;
  int *next;
  void init(unsigned long);
  void free();
  void link(int, int);
  void cut(int, int);
  bool empty(int);
  int first(int);
};
} // namespace DynamicGraph

#endif // LIST_H
