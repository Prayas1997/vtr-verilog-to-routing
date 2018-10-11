#ifndef FASM_H
#define FASM_H

#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "netlist_walker.h"

class FasmWriterVisitor : public NetlistVisitor {
    enum LutMode {
      NO_LUT = 0,
      LUT = 1,
      SPLIT_LUT = 2,
    };

  public:
      FasmWriterVisitor(std::ostream& f);

  private:
      void visit_top_impl(const char* top_level_name) override;
      void visit_open_impl(const t_pb* atom) override;
      void visit_atom_impl(const t_pb* atom) override;
      void visit_clb_impl(ClusterBlockId blk_id, const t_pb* clb) override;
      void visit_all_impl(const t_pb_route *top_pb_route, const t_pb* pb,
          const t_pb_graph_node* pb_graph_node) override;
      void finish_impl() override;

  private:
      void output_fasm_features(std::string features);
      void check_interconnect(const t_pb_route *pb_route, int inode);
      void output_fasm_mux(std::string fasm_mux, t_interconnect *interconnect, t_pb_graph_pin *mux_input_pin);
      void walk_routing();
      std::string build_clb_prefix(const t_pb_graph_node* pb_graph_node) const;
      void setup_split_lut(std::string fasm_lut);
      int find_lut_idx(t_pb_graph_node* pb_graph_node);

      std::ostream& os_;

      t_pb_graph_node *root_clb_;
      bool current_blk_has_prefix_;
      t_type_ptr blk_type_;
      std::string blk_prefix_;
      std::string clb_prefix_;
      ClusterBlockId current_blk_id_;
      std::vector<t_pb_graph_pin**> pb_graph_pin_lookup_from_index_by_type_;

      LutMode lut_mode_;
      std::string lut_prefix_;
      std::vector<std::string> lut_parts_;
      const t_pb_route *pb_route_;
      const t_pb_graph_node* lut_node_;
};

#endif  // FASM_H
