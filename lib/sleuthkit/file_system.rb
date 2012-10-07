# -*- coding: utf-8 -*-
module Sleuthkit
  module FileSystem
    class System
      include ::Sleuthkit

      def tsk_fsstat(file)
        file.puts 'Method returns.'
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
        puts "#{name_or_inum.class} recieved"
        puts "Ruby #find_directory looking for #{name_or_inum}"
        result = nil
        case name_or_inum
          when String
            result = self.find_directory_by_name(name_or_inum)
          when Fixnum
            result = self.find_directory_by_inum(name_or_inum)
          else
            result = nil
        end
        return result
      end
    
      def find_directory_by_name(name)
        puts "Ruby #find_directory_by_name looking for #{name}"
        result = self.open_directory_by_name(name)
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

    class Directory
      include ::Sleuthkit
      attr_reader :name, :inum
    
      def addr
        @inum
      end
      #class methods
      #private 
      def self.parse_opts(opts)
        presets = { :type_flag => 0, :offset => 0 }      
        presets.each_pair do |key, val|
          unless opts.has_key?(key) then opts[key] = val end
        end
        return opts
      end
    end
    
    class FileData
      attr_reader :name
      include ::Sleuthkit
      
    end
    class FileMeta
      include ::Sleuthkit
      FORMATTABLE_ATTRS = [ :atime, :crtime, :ctime, :mtime ]
      attr_reader :addr, :atime, :atime_nano, :content_len, :content_ptr, :crtime, :crtime_nano, :ctime,
      :ctime_nano, :gid, :link, :mode, :mtime, :mtime_nano, :nlink, :seq, :size, :tag
      
      # wrapper methods to format attributes representing times
      FORMATTABLE_ATTRS.each do |att|
        define_method att do |*opts|
          opts.empty? ? opts = :cooked : opts = opts.first.to_sym
          if (opts.to_sym == :raw)
            instance_variable_get("@"+ att.to_s)
          else
            Time.at(instance_variable_get("@"+ att.to_s))
          end
        end
      end
      
    end
    class FileName
      include ::Sleuthkit
      attr_reader :meta_addr, :meta_seq, :name, :name_size, :parent_addr, :shrt_name, :shrt_name_size, :tag
      
    end
  end
end
