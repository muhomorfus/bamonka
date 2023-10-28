<template>
  <main class="container">
      <br>

    <div class="row">
      <div class="col-md-2"></div>
      <div class="col-md-8">

        <div class="card mb-3">
          <div class="card-body">
            <h1>Editing «{{ department.name }}»</h1>

            <form @submit.prevent="saveDepartment">
              <div class="mb-3">
                <label for="name" class="form-label">Name</label>
                <input required type="text" class="form-control" id="name"
                       placeholder="Business department" v-model="department.name">
              </div>
              <div class="mb-3">
                <label for="description" class="form-label">Description</label>
                <textarea required class="form-control" id="description" rows="3" v-model="department.description"></textarea>
              </div>

              <div class="mb-3">
                <label for="parent" class="form-label">Parent department</label>
                <select required class="form-select" id="parent" aria-label="Select parent department" v-model="department.parent_id">
                  <option :value="null" selected="true">No parent department</option>
                  <option v-for="d in departments" :key="d.id" :value="d.id">{{ d.name }}</option>
                </select>
              </div>

              <button class="btn btn-primary" type="submit">Save</button>
            </form>
          </div>
        </div>

      </div>
      <div class="col-md-2"></div>
    </div>
  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";
import ProfileShort from "@/components/ProfileShort.vue";
import department from "../store/modules/department";

export default defineComponent({
  components: {ProfileShort},
  methods: {
    ...mapActions(['loadDepartment', 'saveDepartment', 'loadDepartments']),
  },
  computed: {
    ...mapGetters(['department', 'departments']),
  },
  created() {
    this.loadDepartments()
    this.loadDepartment(this.$route.params.id.toString())
  },
});
</script>