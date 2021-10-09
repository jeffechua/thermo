<template>
  <div class="item-box">
    <div class="contents">
      <table>
        <field
          label="name"
          type="text"
          v-model="element_.name"
          :running="running"
        />
        <tr>
          <th>basis:</th>
          <td v-if="running">{{ element_.basis }}</td>
          <td v-else>
            <select v-model="element_.basis" @change="onCompositionChanged">
              <option
                v-for="basis in data.bases"
                :key="basis.id"
                :value="basis.id"
              >
                {{ basis.name }}
              </option>
            </select>
          </td>
        </tr>
        <tr>
          <th>type:</th>
          <td v-if="running">{{ element_.type }}</td>
          <td v-else>
            <select v-model="element_.type" @change="onTypeChanged">
              <option v-for="(value, key) in specTree" :key="key" :value="key">
                {{ key }}
              </option>
            </select>
          </td>
        </tr>
        <tr>
          <th>format:</th>
          <td v-if="running">{{ element_.format }}</td>
          <td v-else>
            <select v-model="element_.format">
              <option
                v-for="format of specTree[element_.type]"
                :key="format"
                :value="format"
              >
                {{ format }}
              </option>
            </select>
          </td>
        </tr>
        <field
          v-if="element_.format.includes('P')"
          label="pressure (Pa)"
          type="positive number"
          v-model="element_.P"
          :running="running"
        />
        <field
          v-if="element_.format.includes('V')"
          label="volume (m³)"
          type="positive number"
          v-model="element_.V"
          :running="running"
        />
        <field
          v-if="element_.format.includes('T')"
          label="temperature (K)"
          type="positive number"
          v-model="element_.T"
          :running="running"
        />
        <field
          v-if="element_.format.includes('H')"
          label="phys. enthalpy (J)"
          type="number"
          v-model="element_.H"
          :running="running"
        />
        <!-- eslint-disable vue/no-use-v-if-with-v-for -->
        <!-- putting v-if in a wrapper breaks the table/tr association -->
        <field
          v-if="element_.format.includes('n')"
          v-for="(id, i) in derefID(element_.basis, data.bases).components"
          :key="id"
          :label="derefID(id, data.species).name + ' (mol)'"
          :running="running"
          v-model="element_.ns[i]"
        />
        <field
          v-if="element_.format.includes('x')"
          v-for="(id, i) in derefID(element_.basis, data.bases).components"
          :key="id"
          :label="derefID(id, data.species).name + ' (mol%)'"
          :running="
            running ||
            i == derefID(element_.basis, data.bases).components.length - 1
          "
          v-model="element_.xs[i]"
          @change="onCompositionChanged"
        />
        <!-- eslint-enable vue/no-use-v-if-with-v-for -->
      </table>
    </div>
    <list-nav
      v-if="!running"
      :first="first"
      :last="last"
      @moveup="$emit('moveup')"
      @delete="$emit('delete')"
      @movedown="$emit('movedown')"
    />
  </div>
</template>

<script>
import ListNav from "./ListNav.vue";
import Field from "./Field.vue";
import { derefID } from "../utils.js";

export default {
  name: "SpeciesSpec",
  props: ["element", "running", "first", "last", "data"],
  emits: ["moveup", "delete", "movedown"],
  components: { ListNav, Field },
  data() {
    return {
      // evilly bypass the prop mutation restriction
      element_: this.element,
      specTree: {
        SourceSink: ["PTx"],
        IsochoricCell: ["PVTx", "VHn"],
        IsobaricCell: ["PVTx", "PHn", "PTn"],
      },
    };
  },
  methods: {
    derefID: derefID,
    onTypeChanged(event) {
      if (!this.specTree[event.target.value].includes(this.element_.format))
        this.element_.format = this.specTree[event.target.value][0];
    },
    onCompositionChanged() {
      let basis = derefID(this.element_.basis, this.data.bases);
      let sum = 0;
      for (let i = 0; i < basis.components.length - 1; i++)
        sum += this.element_.xs[i];
      this.element_.xs[basis.components.length - 1] = Math.round((1.0 - sum)*1e8)/1e8;
    },
  },
};
</script>