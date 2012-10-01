# -*- coding: utf-8 -*-
module Sleuthkit
  class FileSystem
    include ::Sleuthkit

    def tsk_fsstat(file)
      file.puts 'Hi Matt!'
    end
    def print_tsk_fsstat(report = "")
      # require 'ruby-debug'; debugger

      if report.kind_of?( IO )
        self.call_tsk_fsstat(report)
      elsif report.kind_of?(String)
        r, w = IO.pipe
        self.call_tsk_fsstat(w)
        w.close
        report << r.read
        r.close
        return report
      else
        raise ArgumentError, "arg1 should be IO, File or String object."
      end
    end
    
    private
    def parse_opts(opts)
      presets = { :type_flag => 0 }      
      presets.each_pair do |key, val|
        unless opts.has_key?(key) then opts[key] = val end
      end
      return opts
    end
  end
end
