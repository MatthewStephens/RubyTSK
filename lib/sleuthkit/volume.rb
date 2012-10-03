# -*- coding: utf-8 -*-
module Sleuthkit
  module Volume 
    class System
      # include ::Sleuthkit
      def [](i)
        case i
        when Fixnum
          self.parts[i] || nil
        else
          super
        end
      end
      private
      def parse_opts(opts)
        presets = { :offset => 0, :type_flag => 0 }      
        presets.each_pair do |key, val|
          unless opts.has_key?(key) then opts[key] = val end
        end
        return opts
      end
    end
  
    class Partition
      # include ::Sleuthkit
    end
  end
end
