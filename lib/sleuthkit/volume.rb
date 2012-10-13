# -*- coding: utf-8 -*-
module Sleuthkit
  module Volume 
    def self.type_print
      report=""
      r, w = IO.pipe
      self.return_type_list(w)
      w.close
      report << r.read
      r.close
      return report
    end
    class System
      include ::Sleuthkit
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
      include ::Sleuthkit
    end
  end
end
