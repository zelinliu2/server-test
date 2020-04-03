#!/usr/bin/env ruby


require 'optparse'

options = {}
options[:thread_count] = 10
options[:my_port] = 13000
options[:their_host] = '127.0.0.1'
options[:their_port] = 30000
options[:binary_name] = 'fire_up_connectors'


OptionParser.new do |opts|
  opts.banner = "Usage: __FILE__ --thread_count= --my_port= --their_host= --their_port= --binary_name"
  opts.on("--thread_count=THREAD_COUNT") {|v| options[:thread_count] = v}
  opts.on("--my_port=MY_PORT") {|v| options[:my_port] = v}
  opts.on("--their_host=THEIR_HOST") {|v| options[:their_host] = v}
  opts.on("--their_port=THEIR_PORT") {|v| options[:their_port] = v}
  opts.on("--binary_name=BINARY_NAME") {|v| options[:binary_name] = v}
  opts.separator ''
  opts.separator '################################'
  opts.separator ''
  opts.on("--default"){puts options; exit 0}
end.parse!

str = "g++ main.cpp connector.cpp -lpthread -std=c++17 -o #{options[:binary_name]}"
puts "Executing:\n #{str}"
system str
abort "Error g++ code:#{$?.exitstatus}" unless $?.success?
puts 'Success!'

str = "./#{options[:binary_name]} #{options[:thread_count]} #{options[:my_port]} #{options[:their_host]} #{options[:their_port]}"
puts 'Executing:\n #str'
system str
abort "Error binary #{options[:binary_name]} code:#{$?.exitstatus}" unless $?.success?
puts 'Success!'
