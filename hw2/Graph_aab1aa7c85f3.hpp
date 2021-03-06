#ifndef CME212_GRAPH_HPP
#define CME212_GRAPH_HPP


/** @file Graph.hpp
 * @brief An undirected graph type
 */

#include <algorithm>
#include <vector>
#include <cassert>

#include "CME212/Util.hpp"
#include "CME212/Point.hpp"


/** @class Graph
 * @brief A template for 3D undirected graphs.
 *
 * Users can add and retrieve nodes and edges. Edges are unique (there is at
 * most one edge between any pair of distinct nodes).
 */
template <typename V, typename E> class Graph {
 private:

  // HW0: YOUR CODE HERE
  // Use this space for declarations of important internal types you need
  // later in the Graph's definition.
  // (As with all the "YOUR CODE HERE" markings, you may not actually NEED
  // code here. Just use the space if you need it.)

 public:

  //
  // PUBLIC TYPE DEFINITIONS
  //

  /** Type of this graph. */
  using graph_type = Graph;

  /** Predeclaration of Node type. */
  class Node;

  /** Synonym for Node (following STL conventions). */
  using node_type = Node;

  /** HW1: Synonym for node value time **/
  using node_value_type = V;


  /** Predeclaration of Edge type. */
  class Edge;
  /** Synonym for Edge (following STL conventions). */
  using edge_type = Edge;
  using edge_value_type = E;
  /** Type of node iterators, which iterate over all graph nodes. */
  class NodeIterator;
  /** Synonym for NodeIterator */
  using node_iterator = NodeIterator;

  /** Type of edge iterators, which iterate over all graph edges. */
  class EdgeIterator;
  /** Synonym for EdgeIterator */
  using edge_iterator = EdgeIterator;

  /** Type of incident iterators, which iterate incident edges to a node. */
  class IncidentIterator;
  /** Synonym for IncidentIterator */
  using incident_iterator = IncidentIterator;

  /** Type of indexes and sizes.
      Return type of Graph::Node::index(), Graph::num_nodes(),
      Graph::num_edges(), and argument type of Graph::node(size_type) */
  using size_type = unsigned;

  //
  // CONSTRUCTORS AND DESTRUCTOR
  //

  /** Construct an empty graph. */
  Graph() : nodes_(), adjacency_matrix()
 {

  }

  /** Default destructor */
  ~Graph() = default;

  //
  // NODES
  //

  /** @class Graph::Node
   * @brief Class representing the graph's nodes.
   *
   * Node objects are used to access information about the Graph's nodes.
   */
  class Node: private totally_ordered <Node> {
   public:
    /** Construct an invaold_last node.
     *
     * Vaold_last nodes are obtained from the Graph class, but it
     * is occasionally useful to declare an @i invaold_last node, and assign a
     * vaold_last node to it later. For example:
     *
     * @code
     * Graph::node_type x;
     * if (...should pick the first node...)
     *   x = graph.node(0);
     * else
     *   x = some other node using a complicated calculation
     * do_something(x);
     * @endcode
     */
    Node() {
      // HW0: YOUR CODE HERE
    }

    /** Return this node's position. */
    const Point& position() const {
      // HW0: YOUR CODE HERE
      //TODO: assert here
      return ((*graph_).nodes_[node_id].first);
    }

    Point& position() {
      // HW0: YOUR CODE HERE
      //TODO: assert here
      return ((*graph_).nodes_[node_id].first);
    }

    /** Return this node's index, a number in the range [0, graph_size). */
    size_type index() const {
      // HW0: YOUR CODE HERE
      //TODO: assert here
      return node_id;
    }

    // HW1: YOUR CODE HERE
    // Supply definitions AND SPECIFICATIONS for:
    // node_value_type& value();
    // const node_value_type& value() const;
    // size_type degree() const;
    // incident_iterator edge_begin() const;
    // incident_iterator edge_end() const;


    /** returns an instance of incident_iterator pointing to the first valid node */
    incident_iterator edge_begin() const {
        return IncidentIterator(graph_, node_id, 0);
    }
    /** returns an instance of incident_iterator pointing to the first invalid node */
    incident_iterator edge_end() const {
        return IncidentIterator(graph_, node_id, degree());
    }
    /** returns the non-const value of the node */
    node_value_type& value(){
        return (*graph_).nodes_[node_id].second;
    }
    /** returns const value of the node */
    const node_value_type& value() const {
        return (*graph_).nodes_[node_id].second;
    }
    /** returns how many undirected edges the node is incident on */
    size_type degree() const {
      return graph_->adjacency_matrix[node_id].size();
    }

    /** Test whether this node and @a n are equal.
     *
     * Equal nodes have the same graph and the same index.
     */
    bool operator==(const Node& n) const {
      // HW0: YOUR CODE HERE
      if (node_id == n.node_id and graph_ == n.graph_){
          return true;
      }
      else {
          return false;
      }
    }

    /** Test whether this node is less than @a n in a global order.
     *
     * This ordering function is useful for STL containers such as
     * std::map<>. It need not have any geometric meaning.
     *
     * The node ordering relation must obey trichotomy: For any two nodes x
     * and y, exactly one of x == y, x < y, and y < x is true.
     */
    bool operator<(const Node& n) const {
      // HW0: YOUR CODE HERE
      //TODO: assert here
      if (graph_ == n.graph_ and node_id < n.node_id){
          return true;
      }
      else if (graph_ < n.graph_) {
          return true;
      }
      else {
          return false;
      }
    }

   private:
    // Allow Graph to access Node's private member data and functions.
    friend class Graph;
    // HW0: YOUR CODE HERE
    // Use this space to declare private data members and methods for Node
    // that will not be visible to users, but may be useful within Graph.
    // i.e. Graph needs a way to construct vaold_last Node objects

    //graph_ is a pointer that points to the Graph the node belongs to

    Graph* graph_;

    //node_id is the index of this Node
    size_type node_id;

    //Construct a vaold_last node:
    Node(const Graph* graph, size_type node_id) : graph_(const_cast<Graph*>(graph)),
        node_id(node_id) {}

};

  /** Return the number of nodes in the graph.
   *
   * Complexity: O(1).
   */
  size_type size() const {
    // HW0: YOUR CODE HERE
    return nodes_.size();
  }

  /** Synonym for size(). */
  size_type num_nodes() const {
    return size();
  }

  /** Add a node to the graph, returning the added node.
   * @param[in] position The new node's position
   * @post new num_nodes() == old num_nodes() + 1
   * @post result_node.index() == old num_nodes()
   *
   * Complexity: O(1) amortized operations.
   */
  Node add_node(const Point& position, const node_value_type& myval = node_value_type()) {


    nodes_.push_back(std::make_pair(position, myval));
    //std::vector<size_type> auxadj;
    adjacency_matrix.push_back({});
    return Node(this, size() - 1);
  }

  /** Determine if a Node belongs to this Graph
   * @return True if @a n is currently a Node of this Graph
   *
   * Complexity: O(1).
   */
  bool has_node(const Node& n) const {
    if (n.graph_ == this and n.node_id < size()) {
      return true;
    }
    else {
      return false;
    }

  }

  /** Return the node with index @a i.
   * @pre 0 <= @a i < num_nodes()
   * @post result_node.index() == i
   *
   * Complexity: O(1).
   */
  Node node(size_type i) const {
    return Node(this, i);
  }


  /** Remove node n from the graph he belongs to.
   * @post new() graph.num_edges() = old() graph.num_edges - n.degree();
   * @post new() graph.num_nodes() = old() graph.num_nodes() - 1;
   * 
   *
   * Complexity: O(n.degree()^2).
   * invalidates node with id = n.node_id
   * invalidates node with id = old() graph.num_nodes() - 1;
   * edges incident on n;
   * corresponding node_iterators and edge_iterators;
   */
  size_type remove_node(const Node& n){
    auto g = n.graph_;
    if(!(*g).has_node(n)){
      return 0;
    }
    size_type nodeid = 0;
    nodeid = n.node_id;
    size_type old_last = 0;
    old_last = (*g).num_nodes() - 1;
    //std::cout << nodeid << std::endl;
    std::cout << " " << std::flush;

    (*g).nodes_[nodeid] = (*g).nodes_[old_last];
    (*g).nodes_.pop_back();
    for(size_type i = 0; i < (*g).adjacency_matrix[nodeid].size(); ++i){
      size_type aux1 = adjacency_matrix[nodeid][i].first;
      for(size_type j = 0; j < (*g).adjacency_matrix[aux1].size(); ++j){
        if((*g).adjacency_matrix[aux1][j].first == nodeid){
          (*g).adjacency_matrix[aux1][j] = (*g).adjacency_matrix[aux1].back();
          (*g).adjacency_matrix[aux1].pop_back();
        }
      }
    }
    (*g).adjacency_matrix[nodeid] = (*g).adjacency_matrix[old_last];
    (*g).adjacency_matrix.pop_back();
    for(size_type i = 0; i < (*g).adjacency_matrix[nodeid].size(); ++i){
      size_type aux2 = (*g).adjacency_matrix[nodeid][i].first;
      for(size_type j = 0; j < (*g).adjacency_matrix[aux2].size(); ++j){
        if((*g).adjacency_matrix[aux2][j].first == old_last){
          (*g).adjacency_matrix[aux2][j].first = nodeid;
        }
      }
    }
    return 1;
  }

    /** Remove node *n_it from the graph he belongs to.
   * @post new() graph.num_edges() = old() graph.num_edges - n.degree();
   * @post new() graph.num_nodes() = old() graph.num_nodes() - 1;
   *
   *
   * Complexity: O(n.degree()^2).
   * invalidates node with id = n.node_id
   * invalidates node with id = old() graph.num_nodes() - 1;
   * edges incident on n;
   * corresponding node_iterators and edge_iterators;
   */
  node_iterator remove_node(node_iterator n_it){
    remove(*n_it);
    return n_it;
  }

  /** Remove edge between node @a a and node @a b
   * @post new edge.graph_ doesn't have edge(a,b)
   * @post if old() edge.graph_ has_edge(a,b): new() graph.num_edges() =
   * old() graph.num_edges() -1;
   *
   *
   * Complexity: O(a.degree() + b.degree())
   * invalidates edge(a.node_id, b.node_id)
   * invalidates edge(b.node_id, a.node_id)
   * corresponding edge_iterators
   */
  size_type remove_edge(const Node& a, const Node& b){
  if(!has_edge(a, b)){
      return 0;
    }
    size_type id_a = a.node_id;
    size_type id_b = b.node_id;

    for(size_type i = 0; i < adjacency_matrix[id_a].size(); i++){
      if(adjacency_matrix[id_a][i].first == id_b){
        adjacency_matrix[id_a][i] = adjacency_matrix[id_a][adjacency_matrix[id_a].size() -1];
        adjacency_matrix[id_a].pop_back();
      }
    }
    for(size_type i = 0; i < adjacency_matrix[id_b].size(); i++){
      if(adjacency_matrix[id_b][i].first == id_a){
        adjacency_matrix[id_b][i] = adjacency_matrix[id_b][adjacency_matrix[id_b].size() -1];
        adjacency_matrix[id_b].pop_back();
      }
    }
    if (!has_edge(a,b)){
      return 1;
    }
     return 0;
  }
  /** Remove Edge @a edge
   * @post new edge.graph_ doesn't have edge(a,b)
   * @post if old() edge.graph_ has_edge(a,b): new() graph.num_edges() =
   * old() graph.num_edges() -1;
   *
   *
   * Complexity: O(a.degree() + b.degree())
   * invalidates edge(a.node_id, b.node_id)
   * invalidates edge(b.node_id, a.node_id)
   * corresponding edge_iterators
   */
  size_type remove_edge(const Edge& edge){
    return remove_edge(edge.node1(), edge.node2());
  }

  /** Remove Edge @a *e_it
   * @post new edge.graph_ doesn't have edge(a,b)
   * @post if old() edge.graph_ has_edge(a,b): new() graph.num_edges() =
   * old() graph.num_edges() -1;
   *
   *
   * Complexity: O(a.degree() + b.degree())
   * invalidates edge(a.node_id, b.node_id)
   * invalidates edge(b.node_id, a.node_id)
   * corresponding edge_iterators
   */
  edge_iterator remove_edge(edge_iterator e_it){
    remove_edge(*e_it);
    e_it.makeIteratorGreatAgain();
    return e_it;
  }


  //
  // EDGES
  //

  /** @class Graph::Edge
   * @brief Class representing the graph's edges.
   *
   * Edges are order-insensitive pairs of nodes. Two Edges with the same nodes
   * are considered equal if they connect the same nodes, in either order.
   */
  class Edge: private totally_ordered <Edge> {
   public:
    /** Construct an invaold_last Edge. */
    Edge() {
      // nothing to do here!
    }

    void initialize(edge_value_type val){
      (*graph_).adjacency_matrix[index_1][aux_index_2].second = val;
      symmetric_edge().value() = val;
    }

    edge_value_type& value(){
        return (*graph_).adjacency_matrix[index_1][aux_index_2].second;
    }

    const edge_value_type& value() const {
        return (*graph_).adjacency_matrix[index_1][aux_index_2].second;
    }



    /** Return a node of this Edge */
    Node node1() const {
      return Node(graph_, index_1);
    }

    /** Return the other node of this Edge */
    Node node2() const {
      //given how I am storing the second node I need to use the fetch func
      return Node(graph_, fetch_node2_id());
    }

    /** Test whether this edge and @a e are equal.
     *
     * Equal edges represent the same undirected edge between two nodes.
     */
    bool operator==(const Edge& e) const {
      if (
         graph_ == e.graph_ and
         ((index_1 == e.index_1 and fetch_node2_id() == e.fetch_node2_id())
         or
         (fetch_node2_id() == e.index_1 and index_1 == e.fetch_node2_id())))
         {
         return true;
         }
      else {
         return false;
      }
    }

    /** Test whether this edge is less than @a e in a global order.
     *
     * This ordering function is useful for STL containers such as
     * std::map<>. It need not have any interpretive meaning.
     */
    bool operator<(const Edge& e) const {
      size_type auxmax = std::max(index_1, fetch_node2_id());
      size_type auxmin = std::min(index_1, fetch_node2_id());
      size_type newmax = std::max(e.index_1, e.fetch_node2_id());
      size_type newmin = std::min(e.index_1, e.fetch_node2_id());
      if ((graph_ < e.graph_) or ((graph_ == e.graph_) 
          and ((auxmin < newmin)
                              or
    	 ((auxmin == newmin) and (auxmax < newmax)))
          )) {
  	  return true;
  	} else {
    return false;
     }
    }

  Edge symmetric_edge() const {
    size_type node2 = fetch_node2_id();
    //size_type found_in = 0;
    for(size_type found_in = 0; found_in < (*graph_).adjacency_matrix[node2].size(); found_in++){
      if( (*graph_).adjacency_matrix[node2][found_in].first == index_1){
        return Edge(graph_, node2, found_in);
      }
    }
    return Edge();
  }

   double length() const{
     return norm(node1().position() - node2().position());
   }

   private:
    // Allow Graph to access Edge's private member data and functions.
    friend class Graph;
    // HW0: YOUR CODE HERE
    // Use this space to declare private data members and methods for Edge
    // that will not be visible to users, but may be useful within Graph.
    // i.e. Graph needs a way to construct vaold_last Edge objects


    //graph
    Graph* graph_;
    //node 1
    size_type index_1;
    //node 2
    //NOTE: this is not a node id, this is the index in the corresponding
    // vector with index_1 where the index of the second node is stored
    size_type aux_index_2;

    //fetch node 2 id
    size_type fetch_node2_id() const{
        return graph_->adjacency_matrix[index_1][aux_index_2].first;
    }

  //vaold_last edge constructor
  Edge (const Graph* graph, size_type index_1, size_type aux_index_2) :
      graph_(const_cast<Graph*>(graph)), index_1(index_1), aux_index_2(aux_index_2)
      {}
  };

  /** Return the total number of edges in the graph.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  size_type num_edges() const {
    return std::distance(edge_begin(),edge_end());
  }

  /** Return the edge with index @a i.
   * @pre 0 <= @a i < num_edges()
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  Edge edge(size_type i) const {
    return *std::next(edge_begin(), i);
  }


  /** Test whether two nodes are connected by an edge.
   * @pre @a a and @a b are vaold_last nodes of this graph
   * @return True if for some @a i, edge(@a i) connects @a a and @a b.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  bool has_edge(const Node& a, const Node& b) const {
    size_type a_id = a.node_id;
    size_type b_id = b.node_id;

    for (size_type i = 0; i < adjacency_matrix[a_id].size(); ++i) {
      if (adjacency_matrix[a_id][i].first == b_id) {
        return true; }
    }
    return false;
  }

//}
  /** Add an edge to the graph, or return the current edge if it already exists.
   * @pre @a a and @a b are distinct vaold_last nodes of this graph
   * @return an Edge object e with e.node1() == @a a and e.node2() == @a b
   * @post has_edge(@a a, @a b) == true
   * @post If old has_edge(@a a, @a b), new num_edges() == old num_edges().
   *       Else,                        new num_edges() == old num_edges() + 1.
   *
   * Can invaold_lastate edge indexes -- in other words, old edge(@a i) might not
   * equal new edge(@a i). Must not invaold_lastate outstanding Edge objects.
   *
   * Complexity: No more than O(num_nodes() + num_edges()), hopefully less
   */
  Edge add_edge(const Node& a, const Node& b) {
    // HW0: YOUR CODE HERE
    size_type a_id = a.node_id;
    size_type b_id = b.node_id;

    for(size_type i = 0; i < adjacency_matrix[a_id].size(); ++i){
      if(adjacency_matrix[a_id][i].first == b_id) {
        return Edge(this, a_id, i);
      }
    }

      adjacency_matrix[a_id].push_back(std::make_pair(b_id, edge_value_type()));
      adjacency_matrix[b_id].push_back(std::make_pair(a_id, edge_value_type()));

      //std::pair<size_type, size_type> auxpair(a_id, adjacency_matrix[a_id].size()-1);
      //ordered_edges.push_back(auxpair);

    return Edge(this, a_id, adjacency_matrix[a_id].size() - 1);
  }

  /** Remove all nodes and edges from this graph.
   * @post num_nodes() == 0 && num_edges() == 0
   *
   * Invaold_lastates all outstanding Node and Edge objects.
   */
  void clear() {
    nodes_.clear();
    adjacency_matrix.clear();
    //ordered_edges.clear();
  }

  //
  // Node Iterator
  //

  /** @class Graph::NodeIterator
   * @brief Iterator class for nodes. A forward iterator. */
  class NodeIterator: private equality_comparable <NodeIterator>{
   public:
    // These type definitions let us use STL's iterator_traits.
    using value_type        = Node;                     // Element type
    using pointer           = Node*;                    // Pointers to elements
    using reference         = Node&;                    // Reference to elements
    using difference_type   = std::ptrdiff_t;           // Signed difference
    using iterator_category = std::input_iterator_tag;  // Weak Category, Proxy

    /** Construct an invaold_last NodeIterator. */
    NodeIterator(){}

    /** NodeIterator dereference operator */
    Node operator*() const {
        return Node(graph_,current_);
    }

    /** NodeIterator increment operator */
    NodeIterator& operator++(){
      ++current_;
      return *this;

    }
    /** NodeIterator equality operator */
    bool operator==(const NodeIterator& otheriter) const {
        return (graph_ == otheriter.graph_ and current_ == otheriter.current_);
    }

   private:
    friend class Graph;
    Graph* graph_;
    size_type current_;

    /** NodeIterator private vaold_last constructor*/
    NodeIterator(const Graph* graph, size_type current)
     : graph_(const_cast<Graph*>(graph)), current_(current){}
  };

  /** instantiate a node_iterator */
  node_iterator node_begin() const {
      node_iterator nodeiter = NodeIterator(this, 0);
      return nodeiter;
  }
  /** instantiate a node_iterator */
  node_iterator node_end() const {
      node_iterator nodeiter = NodeIterator(this, size());
      return nodeiter;
  }
  //
  // Incident Iterator
  //

  /** @class Graph::IncidentIterator
   * @brief Iterator class for edges incident to a node. A forward iterator. */
  class IncidentIterator: private equality_comparable <IncidentIterator> {
   public:
    // These type definitions let us use STL's iterator_traits.
    using value_type        = Edge;                     // Element type
    using pointer           = Edge*;                    // Pointers to elements
    using reference         = Edge&;                    // Reference to elements
    using difference_type   = std::ptrdiff_t;           // Signed difference
    using iterator_category = std::input_iterator_tag;  // Weak Category, Proxy

    /** Construct an invaold_last IncidentIterator. */
    IncidentIterator() {
    }

    // HW1 #3: YOUR CODE HERE
    // Supply definitions AND SPECIFICATIONS for:
    // Edge operator*() const
    // IncidentIterator& operator++()
    // bool operator==(const IncidentIterator&) const

    /** IncidentIterator dereference operator */
    Edge operator*() const {
      return Edge(graph_, main_node_, other_aux_);
    }

    /** IncidentIterator increment operator */
    IncidentIterator& operator++() {
      ++other_aux_;
      return *this;
    }

    /** IncidentIterator equality operator */
    bool operator==(const IncidentIterator& incidentiter) const {
      return (graph_ == incidentiter.graph_ 
              and main_node_ == incidentiter.main_node_
              and other_aux_ == incidentiter.other_aux_);
    } 

   private:
    friend class Graph;

    Graph* graph_;
    size_type main_node_;
    size_type other_aux_;

    /** IncidentIterator private vaold_last constructor */
    IncidentIterator(const Graph* graph, size_type main_node, size_type other_aux) :
    graph_(const_cast<Graph*>(graph)), main_node_(main_node), other_aux_(other_aux){};
    // HW1 #3: YOUR CODE HERE

  };

  //
  // Edge Iterator
  //

  /** @class Graph::EdgeIterator
   * @brief Iterator class for edges. A forward iterator. */
  class EdgeIterator: private equality_comparable <EdgeIterator> {
   public:
    // These type definitions let us use STL's iterator_traits.
    using value_type        = Edge;                     // Element type
    using pointer           = Edge*;                    // Pointers to elements
    using reference         = Edge&;                    // Reference to elements
    using difference_type   = std::ptrdiff_t;           // Signed difference
    using iterator_category = std::input_iterator_tag;  // Weak Category, Proxy

    /** Construct an invaold_last EdgeIterator. */
    EdgeIterator(){
    }

    /** EdgeIterator dereference operator */
    Edge operator*() const {
      return Edge(graph, main_node_, other_aux_);
    }

    /** EdgeIterator increment operator */
    EdgeIterator& operator++(){
      ++other_aux_;
      makeIteratorGreatAgain();
      return *this;
    }

    /** EdgeIterator equality operator */
    bool operator==(const EdgeIterator& edgeiter) const {
      return(graph == edgeiter.graph and main_node_ == edgeiter.main_node_
             and other_aux_ == edgeiter.other_aux_);
    }

   private:
    friend class Graph;
    // HW1 #5: YOUR CODE HERE
    Graph* graph;
    size_type main_node_;
    size_type other_aux_;

    /** this function is the nicest part of the whole homework. It
     ** finds the next vaold_last edge and allowed me to get rid of the edge vector. */
    void makeIteratorGreatAgain() {
      while (true) {
        if (main_node_ == graph->num_nodes()) {
            break;
        }
        else if (other_aux_ == graph->adjacency_matrix[main_node_].size()){
          other_aux_ = 0;
          main_node_++;
        }
        else if ( main_node_ > graph->adjacency_matrix[main_node_][other_aux_].first){
          other_aux_++;
        }
        else {
          break;
        }
      }
    }

    /** EdgeIterator private vaold_last constructor*/
    EdgeIterator(const Graph* g, size_type main_node, size_type other_aux) :
                 graph(const_cast<Graph*>(g)), main_node_(main_node), other_aux_(other_aux){
                  makeIteratorGreatAgain();
                 }

  };

  /** instantiate an EdgeIterator */
  edge_iterator edge_begin() const {
      return EdgeIterator(this,0,0);
  }
  /** instantiate an the first invaold_last EdgeItertor, the args required some thinking */
  edge_iterator edge_end() const {
      return EdgeIterator(this, num_nodes(), 0);
  }


 private:

  std::vector<std::pair<Point, node_value_type>> nodes_;

  //vector of vector storing edge information
  //std::vector<std::vector<size_type>> adjacency_matrix;
  std::vector<std::vector<std::pair<size_type, edge_value_type>>> adjacency_matrix;

  //vector of edges to make edge function fast
  //DEPRECATED: (BEAUTIFUL!)
  //std::vector<std::pair<size_type, size_type>> ordered_edges;
};

#endif // CME212_GRAPH_HPP
