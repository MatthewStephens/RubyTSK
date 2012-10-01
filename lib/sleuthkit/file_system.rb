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
    
    def find_directory(name_or_inum)
      puts "#{str.class} recieved"
      puts "Ruby #find_directory looking for #{name}"
      result = nil
      case str
        when String
          result = self.open_directory_by_name(name_or_inum)
        when Fixnum
          result = self.open_directory_by_inum(name_or_inum)
        else
          result = nil
      end
      return result
    end
    
    def find_directory_by_name(name)
      puts "Ruby #find_directory_by_name looking for #{name}"
      result = self.open_directory_by_inum(name)
    end
    def find_directory_by_inum(n)
      inum = n.to_i
      puts "Ruby #find_directory_by_inum looking for #{inum}"
      result = self.open_directory_by_inum(inum)
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

  class FileSystemDirectory
    include ::Sleuthkit
    
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
