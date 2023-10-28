<template>
  <div class="notification-block">
    <div v-for="error in errors" :key="error"
         class="alert alert-primary alert-dismissible fade show" role="alert">
      {{ error }}
      <button type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"
              @click="hideError(error)"></button>
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import { mapActions, mapGetters } from 'vuex';

export default defineComponent({
  name: 'ErrorsBlock',
  methods: {
    ...mapActions(['showError', 'hideError']),
  },
  computed: {
    ...mapGetters(['errors']),
  },
});
</script>

<style scoped>
.notification-block {
  position: fixed;
  width: 23vw;
  bottom: 10px;
  left: 17px;
}
</style>
