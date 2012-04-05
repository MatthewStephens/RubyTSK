# -*- coding: utf-8 -*-
require 'sleuthkit/version'
require 'tsk4r/tsk4r'

module Sleuthkit
  # tell us about yourself
	def self.version_string
		"Sleuthkit Ruby Binding version #{Sleuthkit::VERSION}"
	end	
	class Image
		def ruby_welcome
			puts "this is a pure ruby method for #{self.class} objects."
		end
	end
end
