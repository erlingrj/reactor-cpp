#ifndef __LF_ROS_WRAPPER_HH_
#define __LF_ROS_WRAPPER_HH_

#include <string>
#include <chrono>

#include "reactor-cpp/reactor.hh"

namespace rclcpp {

    typedef void (*reaction_callback)(void);

    // Timer object. Should implement the rest of the TimerBase API.
    //  this includes changing interval of timer which means that maybe
    //  action is a better mapping    
    class TimerBase {
        TimerBase();
    };

    // Publisher object. This is really an output-port. 
    //  publish should translate to "set" on the port
    class Publisher {
        Publisher();
        void publish(std::string msg);
    };

    // This is really an input port of the reactor. 
    class Subscribtion {
        Subscribtion();
    };


    class RosReactor: public reactor::Reactor {
        RosReactor(std::string name, reactor::Environment *env): reactor::Reactor(name, env) {}

        void assemble() {
            // Assembly is done in create_wall_timer, create_publisher etc
        }
    };

    // An ROS Node implementation targeting LF. It has a Reactor object which it uses
    //  to communicate with LF-world
    class Node {
    public:
        // Constructor should initialize the RosReactor, must also pass in
        //  The correct Environment. Or maybe this can wait until call to spin?
        Node(std::string name);

        // Translate into creating a LF Timer. Should add timer and reaction to the RosReactor object.
        //  Without additional knowledge, we assign precedence to reactions in the order in which they are specified
        TimerBase *create_wall_timer(
            std::chrono::duration<int64_t> period,
            reaction_callback cb
            );

        // Translate into creating an output Port (if it is not already created) of the reactor. Without more info
        //  we must assume that all reactions (already created and created in the future) have this as anti-dependency
        Publisher *create_publisher(std::string topic, size_t unused);

        // This translates into creating an Input Port (if it is not already created) of the reactor.
        //  also create a reaction triggered by this Input Port
        Subscribtion *create_subscription(
            std::string topic, 
            size_t unused, 
            reaction_callback cb
            );
    private:
        RosReactor reactor;
    };

    // init creates the Environment which somehow must be made 
    //  available to all Nodes(RosReactors)
    void init(int argc, char **argv);

    // Spin is actually superflous. When the Node is created it's constructor
    //  will make publishers, subscribers and timers. These correspond to dynamically building Reactor graph
    void spin(Node *node);

    // We do miss a call to start the scheduler. Might have to be added.

    // Shutdown. Not sure, about this.
    void shutdown();


}


#endif