/**
 * @file mass_spring.cpp
 * Implementation of mass-spring system using Graph
 *
 * @brief Reads in two files specified on the command line.
 * First file: 3D Points (one per line) defined by three doubles
 * Second file: Tetrahedra (one per line) defined by 4 indices into the point
 * list
 */

#include <fstream>
#include <chrono>
#include <thread>

#include <tuple>
#include <type_traits>

#include "CME212/SFML_Viewer.hpp"
#include "CME212/Util.hpp"
#include "CME212/Color.hpp"
#include "CME212/Point.hpp"

#include "Graph.hpp"


// Gravity in meters/sec^2
static constexpr double grav = 9.81;
static constexpr double K = 100;
double C = 0;

/** Custom structure of data to store with Nodes */
struct NodeData {
  Point vel;       //< Node velocity
  double mass;     //< Node mass
  NodeData() : vel(0), mass(1) {}
};

/** Custom structure of data to store with Edges */
struct EdgeData {
	double K;  //< String constant
	double L;  //< length of the edge
	EdgeData(): K(0), L(0) {};
};

// Define the Graph type
using GraphType = Graph<NodeData, EdgeData>;
using Node = typename GraphType::node_type;
using Edge = typename GraphType::edge_type;

/** Change a graph's nodes according to a step of the symplectic Euler
 *    method with the given node force.
 * @param[in,out] g      Graph
 * @param[in]     t      The current time (useful for time-dependent forces)
 * @param[in]     dt     The time step
 * @param[in]     force  Function object defining the force per node
 * @return the next time step (usually @a t + @a dt)
 *
 * @tparam G::node_value_type supports ???????? YOU CHOOSE
 * @tparam F is a function object called as @a force(n, @a t),
 *           where n is a node of the graph and @a t is the current time.
 *           @a force must return a Point representing the force vector on
 *           Node n at time @a t.
 */
template <typename G, typename F, typename C>
double symp_euler_step(G& g, double t, double dt, F force, C constraint) {
  // Compute the t+dt position
  for (auto it = g.node_begin(); it != g.node_end(); ++it) {
    auto n = *it;

    // Update the position of the node according to its velocity
    // x^{n+1} = x^{n} + v^{n} * dt
    n.position() += n.value().vel * dt;
  }

  // Apply the constrain to the nodes at time t
  constraint(g, t);

  // Compute the t+dt velocity
  for (auto it = g.node_begin(); it != g.node_end(); ++it) {
    auto n = *it;
    if (n.position() == Point(0,0,0) || n.position() == Point(1,0,0)) {
    	continue;
    }
    // v^{n+1} = v^{n} + F(x^{n+1},t) * dt / m
    n.value().vel += force(n, t) * (dt / n.value().mass);
  }

  return t + dt;
}


/** Force function object for HW2 #1. */
struct Problem1Force {
  /** Return the force applying to @a n at time @a t.
   *
   * For HW2 #1, this is a combination of mass-spring force and gravity,
   * except that points at (0, 0, 0) and (1, 0, 0) never move. We can
   * model that by returning a zero-valued force. */
  template <typename NODE>
  Point operator()(NODE n, double t) const {
    // HW2 #1: YOUR CODE HERE
    // Boundary condition
    if (n.position() == Point(0,0,0) || n.position() == Point(1,0,0)) {
    	return Point(0,0,0);
    }

    // set up with gravity froces
    Point total_force(0, 0, -n.value().mass * grav);
    // compute the sum of string froces from the adjacent nodes
    for (auto ii = n.edge_begin(); ii != n.edge_end(); ++ii) {
    	auto e = *ii;
    	total_force += (-e.value().K) * (e.node1().position() - e.node2().position()) * 
						(e.length() - e.value().L) / e.length();
    }
    return total_force;
  }
};

// Force function of the gravity
struct GravityForce  {
	template <typename NODE>
	Point operator()(NODE n, double t) const{
		return Point(0, 0, -n.value().mass * grav);
	}
};

// Force function of the mass spring
struct MassSpringForce {
	template <typename NODE>
	Point operator()(NODE n, double t) const{
		// set up with gravity froces
	    Point total_force(0, 0, 0);
	    // compute the sum of string froces from the adjacent nodes
	    for (auto ii = n.edge_begin(); ii != n.edge_end(); ++ii) {
	    	auto e = *ii;
	    	total_force += (-e.value().K) * (e.node1().position() - e.node2().position()) * 
							(e.length() - e.value().L) / e.length();
	    }
	    return total_force;
	}
};

// Force function of the dampping force
struct DampingForce {
	template <typename NODE>
	Point operator()(NODE n, double t) const{
		// set up with gravity froces
	    return -C * n.value().vel;
	}
};

// Template function for allowing arbitrary number of force functions
template <std::size_t N, std::size_t I, typename Tuple>
struct evaluate_all_forces
{
    template <typename Node>
    static Point eval(Node n, double t, Tuple const & forces)
    {
        return std::get<I>(forces)(n, t)
            + evaluate_all_forces<N, I + 1, Tuple>::eval(n, t, forces);
    }
};

// End of template functions array.
template <std::size_t N, typename Tuple>
struct evaluate_all_forces<N, N, Tuple> {
  template<typename Node>
  static Point eval(Node , double, Tuple const &) {
    return Point(0,0,0);
  }
};

// Froce function that combines given input force function
template<typename...Forces>
struct combined_force {
  private:
    typedef std::tuple<Forces...> tuple_type;
    tuple_type forces;
  public:
    combined_force(Forces const &... f) : forces(f...) { }
    template<typename Node>
    Point operator()(Node  n, double  t) const {
      return evaluate_all_forces<sizeof...(Forces), 0, tuple_type>::eval(n, t, forces);
    }
};

// Helper function for producing combined_force
template<typename...Forces>
combined_force<typename std::decay<Forces>::type...> make_combined_force(Forces &&... forces) {
  return combined_force<typename std::decay<Forces>::type...>(std::forward<Forces>(forces)...);
}

struct PlainConstraint {
  template<typename G>
  void operator()(G &graph, double t) {
    for (auto ni = graph.node_begin(); ni != graph.node_end(); ++ni) {
      auto n = *ni;
      if (n.position().z < -0.75) {
        n.position().z = -0.75;
      }
    }
  }
};

struct SphereConstraint {
  const Point C = Point(0.5, 0.5, -0.5);
  const double radius = 0.15;

  template<typename G>
  void operator()(G &graph, double t) {
    for (auto ni = graph.node_begin(); ni != graph.node_end(); ++ni) {
      auto n = *ni;
      Point dir = n.position() -C;
      if (norm(dir) < radius) {
        Point v = dir / norm(dir);
        n.position() += v * (radius - norm(dir));
        n.value().vel -= inner_prod(n.value().vel, v) * v;
      } 
    }
  }
};

struct SphereRemoveConstraint {
  const Point C = Point(0.5, 0.5, -0.5);
  const double radius = 0.15;
  template<typename G>
  void operator()(G &g, double t) {
    auto curr = g.node_begin();
    while (curr != g.node_end()) {
      Node n = *curr;
      if (norm(n.position() - C) < radius) {
        curr = g.remove_node(curr);
      } else {
        ++curr;
      }
    }
  }
};


template <std::size_t N, std::size_t I, typename Tuple>
struct evaluate_all_constraints
{
    template <typename G>
    static void eval(G &g, double t, Tuple & constraints)
    {
        std::get<I>(constraints)(g, t);
        evaluate_all_constraints<N, I + 1, Tuple>::eval(g, t, constraints);
    }
};

template <std::size_t N, typename Tuple>
struct evaluate_all_constraints<N, N, Tuple> {
  template<typename G>
  static void eval(G &, double, Tuple &) {
  }
};

template<typename...Constraints>
struct combined_constraints{
  private:
    typedef std::tuple<Constraints...> tuple_type;
    tuple_type constraints;
  public:
    combined_constraints(Constraints ... c) : constraints(c...) { }
    template<typename G>
    void operator()(G & g, double t)  {
      evaluate_all_constraints<sizeof...(Constraints), 0, tuple_type>::eval(g, t, constraints);
    }
};

template<typename...Constraints>
combined_constraints<typename std::decay<Constraints>::type...> make_combined_constraints(Constraints &&... constraints) {
  return combined_constraints<typename std::decay<Constraints>::type...>(std::forward<Constraints>(constraints)...);
}

int main(int argc, char** argv) {
  // Check arguments
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " NODES_FILE TETS_FILE\n";
    exit(1);
  }

  // Construct an empty graph
  GraphType graph;

  // Create a nodes_file from the first input argument
  std::ifstream nodes_file(argv[1]);
  // Interpret each line of the nodes_file as a 3D Point and add to the Graph
  Point p;
  std::vector<typename GraphType::node_type> nodes;
  while (CME212::getline_parsed(nodes_file, p))
    nodes.push_back(graph.add_node(p));

  // Create a tets_file from the second input argument
  std::ifstream tets_file(argv[2]);
  // Interpret each line of the tets_file as four ints which refer to nodes
  std::array<int,4> t;
  while (CME212::getline_parsed(tets_file, t)) {
    graph.add_edge(nodes[t[0]], nodes[t[1]]);
    graph.add_edge(nodes[t[0]], nodes[t[2]]);

    // Diagonal edges: include as of HW2 #2
    graph.add_edge(nodes[t[0]], nodes[t[3]]);
    graph.add_edge(nodes[t[1]], nodes[t[2]]);

    graph.add_edge(nodes[t[1]], nodes[t[3]]);
    graph.add_edge(nodes[t[2]], nodes[t[3]]);
  }

  // HW2 #1 YOUR CODE HERE
  // Set initial conditions for your nodes, if necessary.
  for(auto it = graph.node_begin(); it != graph.node_end(); ++it) {
  	auto n = *it;
  	n.value().vel = Point(0, 0, 0);
  	n.value().mass = 1.0 / graph.num_nodes();
  }
  // set the rest length assuming all edges have the same initial length
  for (auto ei = graph.edge_begin(); ei != graph.edge_end(); ++ei) {
  	auto e = *ei;
  	e.value().K = K;
  	e.value().L = e.length();
  }
  // setup C
  C = 1.0 / graph.num_nodes();

  // Print out the stats
  std::cout << graph.num_nodes() << " " << graph.num_edges() << std::endl;

  // Launch the Viewer
  CME212::SFML_Viewer viewer;
  auto node_map = viewer.empty_node_map(graph);

  viewer.add_nodes(graph.node_begin(), graph.node_end(), node_map);
  viewer.add_edges(graph.edge_begin(), graph.edge_end(), node_map);

  viewer.center_view();

  // We want viewer interaction and the simulation at the same time
  // Viewer is thread-safe, so launch the simulation in a child thread
  bool interrupt_sim_thread = false;
  auto sim_thread = std::thread([&](){

      // Begin the mass-spring simulation
      double dt = 0.0005;
      double t_start = 0;
      double t_end = 5.0;
      auto testForce = make_combined_force(GravityForce(), MassSpringForce(), DampingForce());
      auto testCons = make_combined_constraints(PlainConstraint(), SphereConstraint(), SphereRemoveConstraint());

      for (double t = t_start; t < t_end && !interrupt_sim_thread; t += dt) {
        //symp_euler_step(graph, t, dt, Problem1Force());
      	symp_euler_step(graph, t, dt, testForce, testCons);

        viewer.clear();
        node_map.clear();
        // Update viewer with nodes’ new positions and new edges
        viewer.add_nodes(graph.node_begin(), graph.node_end(), node_map);
        viewer.add_edges(graph.edge_begin(), graph.edge_end(), node_map);

        viewer.set_label(t);

        // These lines slow down the animation for small graphs, like grid0_*.
        // Feel free to remove them or tweak the constants.
        if (graph.size() < 100)
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }

    });  // simulation thread

  viewer.event_loop();

  // If we return from the event loop, we've killed the window.
  interrupt_sim_thread = true;
  sim_thread.join();

  return 0;
}
