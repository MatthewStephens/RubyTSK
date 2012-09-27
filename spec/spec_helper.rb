# figure out where we are being loaded from, warn if loaded more than once
if $LOADED_FEATURES.grep(/spec\/spec_helper\.rb/).any?
    begin
        raise "spec_helper redundant load!"
        rescue => e
        puts <<-MSG
        ===================================================
        It looks like spec_helper.rb has been loaded
        multiple times. Normalize the require to:
        
        require "spec/spec_helper"
        
        Things like File.join and File.expand_path will
        cause it to be loaded multiple times.
        
        Loaded this time from:
        
        #{e.backtrace.join("\n    ")}
        ===================================================
        MSG
    end
    end

require 'rspec'
require 'sleuthkit'
require 'pp'



RSpec.configure do |config|
	config.color_enabled = true
	config.formatter = 'documentation'
end
