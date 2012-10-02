# -*- coding: utf-8 -*-
require 'sleuthkit/image'
require 'sleuthkit/file_system'
require 'sleuthkit/version'
require 'sleuthkit/volume'
require 'tsk4r/tsk4r' # gem install process uses this dir
# require 'sleuthkit/image.rb'

module Sleuthkit
  # require 'ruby-debug'
  require 'pp'
  
  # tell us about yourself
	def self.version_string
		"Sleuthkit Ruby Binding version #{Sleuthkit::VERSION} (TSK version: #{Sleuthkit::TSK_VERSION})"
	end
  def [](sym)
    self.instance_variable_get("@#{sym.to_s}")
  end
  def inspect_object
    #debugger
    h = Hash.new
    self.instance_variables.each do |var|
      h[var.to_s.gsub(/@/, '').to_sym] = instance_variable_get(var)
    end
    h
  end

end
