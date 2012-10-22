# -*- coding: utf-8 -*-
module Sleuthkit
  module FileSystem
    def self.type_print
      report=""
      r, w = IO.pipe
      self.return_type_list(w)
      w.close
      report << r.read
      r.close
      return report
    end
    def self.new(fs)
      return ::Sleuthkit::FileSystem::System.new(fs)
    end
    class System
      include ::Sleuthkit
      SEARCH_METHODS=[ :directory, :file ]

      # these procs are templates to build sets of search methods
      find_proc=Proc.new do |att|
        define_method "find_#{att}" do |name_or_inum|
          result = nil
          case name_or_inum
          when String
            result = self.instance_eval "open_#{att}_by_name(name_or_inum)"
          when Integer
            result = self.instance_eval "open_#{att}_by_inum(name_or_inum)"
          else
            result
          end
        end
      end

      inum_proc=Proc.new do |att|
        define_method "find_#{att}_by_inum" do |inum| 
          result = nil
          result = self.instance_eval "open_#{att}_by_inum(inum)"
        end
      end
      
      name_proc=Proc.new do |att|
        define_method "find_#{att}_by_name" do |name| 
          result = nil
          result = self.instance_eval "open_#{att}_by_name(name)"
        end
      end
      
      SEARCH_METHODS.each do |att|
        self.instance_eval do
          define_method "find_#{att}",         find_proc.call(att)
          define_method "find_#{att}_by_inum", inum_proc.call(att)
          define_method "find_#{att}_by_name", name_proc.call(att)
        end
      end
      
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
      alias_method :stat, :print_tsk_fsstat
      def istat(inum, report=STDOUT, opts={})
        # if opts were passed w/o report, assign report to STDOUT
        if report.is_a?(Hash) && opts.empty? then opts=report; report=STDOUT end

        opts ||= Hash.new
        if report.kind_of?( IO )
          self.call_tsk_istat(inum, report, opts)
        elsif report.kind_of?(String)
          r, w = IO.pipe
          self.call_tsk_istat(inum, w, opts)
          w.close
          report << r.read
          r.close
          return report
        else
          raise ArgumentError, "arg2 should be IO, File or String object."
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
      def return_file_attributes
        attrs= []
        list = *(0..(self.get_number_of_attributes-1))
	      if list == 0 then list=[ 0 ] end
        list.each do |attr_num|
          attrs << Sleuthkit::FileSystem::Attribute.new(self, attr_num)
        end
        attrs.length == 1 ? attrs.first : attrs
      end
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
    class Block
      include ::Sleuthkit
    end
    class FileName
      include ::Sleuthkit
      attr_reader :meta_addr, :meta_seq, :name, :name_size, :parent_addr, :shrt_name, :shrt_name_size, :tag
      
    end
    class Attribute
      include ::Sleuthkit
    end
  end
end
