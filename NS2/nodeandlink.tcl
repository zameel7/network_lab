set ns [new Simulator]

set node0 [$ns node]
set node1 [$ns node]
set node2 [$ns node]
set node3 [$ns node]
set node4 [$ns node]

set link0 [$ns duplex-link $node0 $node1 10Mb 10ms DropTail]
set link1 [$ns duplex-link $node1 $node2 10Mb 10ms DropTail]
set link2 [$ns duplex-link $node2 $node3 10Mb 10ms DropTail]
set link3 [$ns duplex-link $node3 $node4 10Mb 10ms DropTail]

$ns rtproto Static

$ns at 5.0 "$ns halt"

$ns run